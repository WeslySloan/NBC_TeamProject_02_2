#include "AI_Monster/SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Monster"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);

	MonsterDataTable = nullptr;
}

void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	// 예전 테스트 스폰 루프는 제거. 웨이브로 관리.
	StartWaves();
}

void ASpawnVolume::StartWaves()
{
	CurrentWave = 0;
	SpawnedThisWave = 0;
	KilledThisWave = 0;

	GetWorldTimerManager().ClearTimer(WaveTimerHandle);

	// 1웨이브 즉시 시작 (가변 길이이므로 반복타이머 대신 단발 재설정 방식 사용)
	StartNextWave();
}

void ASpawnVolume::StartNextWave()
{
	// 조기 클리어 루트와 충돌 방지
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);

	CurrentWave++;
	WaveStartTimeSec = GetWorld()->GetTimeSeconds();
	SpawnedThisWave = 0;
	KilledThisWave = 0;

	UE_LOG(LogTemp, Log, TEXT("[Wave %d] started."), CurrentWave);

	// 네가 쓰던 패턴 그대로 스폰
	SpawnForCurrentWave_UsingYourPattern();

	// 이번 웨이브 길이로 다음 웨이브 예약(단발)
	ArmNextWaveTimer();
}

float ASpawnVolume::ComputeWaveDuration(int32 WaveIndex) const
{
	// 1웨이브 = WaveDuration
	// 2웨이브 = WaveDuration - 1초
	// 3웨이브 = WaveDuration - 2초 ...
	const float dec = (WaveIndex > 0) ? (WaveIndex - 1) * WaveDurationDecrementPerWave : 0.f;
	return FMath::Max(MinWaveDuration, WaveDuration - dec);
}

void ASpawnVolume::ArmNextWaveTimer()
{
	const float Duration = ComputeWaveDuration(CurrentWave);

	GetWorldTimerManager().SetTimer(
		WaveTimerHandle,
		this,
		&ASpawnVolume::StartNextWave,
		Duration,
		false // 단발
	);

	UE_LOG(LogTemp, Log, TEXT("[Wave %d] duration set to %.1f sec"), CurrentWave, Duration);
}

void ASpawnVolume::SpawnForCurrentWave_UsingYourPattern()
{
	// 이번 웨이브 스폰 수 = Base + (Wave-1) * +5
	const int32 CountToSpawn =
		FMath::Max(0, BaseMonstersPerWave + (CurrentWave - 1) * IncrementPerWave);

	// ────── 네가 쓰던 패턴 그대로 ──────
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	for (int32 i = 0; i < CountToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]); // 첫 볼륨만 사용(원래 코드 유지)
			if (SpawnVolume)
			{
				if (AActor* SpawnedActor = SpawnVolume->SpawnRandomMonster())
				{
					// 처치 수 카운트를 위해 파괴 이벤트 구독
					SpawnedActor->OnDestroyed.AddDynamic(this, &ASpawnVolume::HandleSpawnedActorDestroyed);
					SpawnedThisWave++;
				}
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("[Wave %d] Spawned %d monsters (Base=%d, +%d per wave)"),
		CurrentWave, SpawnedThisWave, BaseMonstersPerWave, IncrementPerWave);
}

AActor* ASpawnVolume::SpawnRandomMonster()
{
	if (FMonsterSpawnRow* SelectedRow = GetRandomMonster())
	{
		if (UClass* ActualClass = SelectedRow->MonsterClass.Get())
		{
			return SpawnMonster(ActualClass);
		}
	}
	return nullptr;
}

FMonsterSpawnRow* ASpawnVolume::GetRandomMonster() const
{
	if (!MonsterDataTable) return nullptr;

	TArray<FMonsterSpawnRow*> AllRows;
	static const FString ContextString(TEXT("MonsterSpawnContext"));
	MonsterDataTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty()) return nullptr;

	float TotalChance = 0.0f;
	for (const FMonsterSpawnRow* Row : AllRows)
	{
		if (Row)
		{
			TotalChance += Row->SpawnChance;
		}
	}

	const float RandValue = FMath::FRandRange(0.0f, TotalChance);
	float AccumulateChance = 0.0f;
	for (FMonsterSpawnRow* Row : AllRows)
	{
		AccumulateChance += Row->SpawnChance;
		if (RandValue <= AccumulateChance)
		{
			return Row;
		}
	}
	return nullptr;
}

FVector ASpawnVolume::GetRandomPointInVolume() const
{
	const FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	const FVector BoxOrigin = SpawningBox->GetComponentLocation();
	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
	);
}

AActor* ASpawnVolume::SpawnMonster(TSubclassOf<AActor> Monster)
{
	if (!Monster) return nullptr;

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
		Monster,
		GetRandomPointInVolume(),
		FRotator::ZeroRotator
	);
	return SpawnedActor;
}

void ASpawnVolume::HandleSpawnedActorDestroyed(AActor* /*DestroyedActor*/)
{
	KilledThisWave = FMath::Clamp(KilledThisWave + 1, 0, SpawnedThisWave);
	const int32 Alive = FMath::Max(0, SpawnedThisWave - KilledThisWave);

	// 전장에 적이 0이면 즉시 다음 웨이브 시작
	if (bSpawnNextWaveWhenCleared && Alive == 0)
	{
		OnAllKilledCheckAndMaybeLevelUp(); // (선택) 레벨업 판정

		// 남은 타이머 취소 후 즉시 다음 웨이브
		GetWorldTimerManager().ClearTimer(WaveTimerHandle);
		UE_LOG(LogTemp, Log, TEXT("[Wave %d] cleared early -> starting next wave immediately."), CurrentWave);
		StartNextWave();
	}
}

void ASpawnVolume::OnAllKilledCheckAndMaybeLevelUp()
{
	const float nowSec = GetWorld()->GetTimeSeconds();
	const float elapsed = nowSec - WaveStartTimeSec;

	// 웨이브별 제한시간
	const float limit =
		FMath::Max(KillAllTimeMin, KillAllTimeLimitWave1 - (CurrentWave - 1) * KillAllTimeReducePerWave);

	// 조건: 웨이브 시간(가변) 이내 + 제한시간 이내
	if (elapsed <= ComputeWaveDuration(CurrentWave) && elapsed <= limit)
	{
		PlayerLevel++;
		UE_LOG(LogTemp, Warning, TEXT("[Wave %d] All killed in %.1fs (Limit=%.1fs) -> LEVEL UP! Lv.%d"),
			CurrentWave, elapsed, limit, PlayerLevel);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[Wave %d] All killed in %.1fs (Limit=%.1fs) -> No level up"),
			CurrentWave, elapsed, limit);
	}
}
