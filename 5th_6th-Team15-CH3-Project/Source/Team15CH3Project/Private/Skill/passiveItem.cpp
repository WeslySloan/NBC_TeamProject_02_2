#include "Skill/passiveItem.h"
#include "PlayerMade/CharacterStatsComponent.h"
#include "PlayerMade/PlayerCharacter.h"
#include "AI_Monster/AI_Monsters.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Skill/Actor/Drone.h"
#include "Kismet/GameplayStatics.h"

ApassiveItem::ApassiveItem()
{
	PassiveSkillData.StackCnt = 0;
	PassiveSkillData.MaxStackCnt = 5;
	PassiveType = EPassiveItemType::None;

	static ConstructorHelpers::FClassFinder<ADrone> DroneBP(TEXT("/Game/SKill/BluePrint/BP_Drone"));
	if (DroneBP.Succeeded())
	{
		DroneClass = DroneBP.Class;
	}
}

void ApassiveItem::BeginPlay()
{
	Super::BeginPlay();
}

void ApassiveItem::PassiveSkillApply(APlayerCharacter* Target)
{
	if (!Target) return;

	UCharacterStatsComponent* Stats = Target->FindComponentByClass<UCharacterStatsComponent>();
	if (!Stats) return;

	if (PassiveSkillData.StackCnt < PassiveSkillData.MaxStackCnt)
	{
		PassiveSkillData.StackCnt++;
	}

	const float Multiplier = 1.0f + (PassiveSkillData.PercentStack / 100.0f) * PassiveSkillData.StackCnt;

	switch (PassiveType)
	{
	case EPassiveItemType::AttackPowerBoost:
		Stats->AttackDamage *= Multiplier;
		break;
	case EPassiveItemType::AttackSpeedBoost:
		Stats->AttackSpeed *= Multiplier;

		break;
	case EPassiveItemType::AttackIncreasingDirection:
		Stats->ProjectileCount += 1;
		break;
	case EPassiveItemType::BulletBoost:

		break;
	case EPassiveItemType::HPBoost: //최대 HP 증가 효과
		Stats->MaxHP *= Multiplier;

		break;
	case EPassiveItemType::MPBoost: //최대 MP 증가 효과
		Stats->MaxMP *= Multiplier;

		break;
	case EPassiveItemType::ExpBoost: //경험치 획득량 증가 적AI 에서 받아올게 필요
	{
		TArray<AActor*> FoundMonsters;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAI_Monsters::StaticClass(), FoundMonsters);

		for (AActor* Actor : FoundMonsters)
		{
			if (AAI_Monsters* AI_Monster = Cast<AAI_Monsters>(Actor))
			{
				AI_Monster->ExpReward *= Multiplier;
			}
		}
		break;
	}

		break;
	case EPassiveItemType::SprintBoost:
		Stats->MoveSpeed = Stats->MoveSpeed * Multiplier;

		if (APlayerCharacter* PC = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
				{
					if (UCharacterMovementComponent* MovementComp = PC->GetCharacterMovement())
					{
						MovementComp->MaxWalkSpeed = Stats->MoveSpeed;
					}
				}

		break;
	case EPassiveItemType::BloodAbsorbing: //현재 체력을 흡혈 효과로 회복하는 패시브
		// 여기서는 스킬 획득 판정만 하고 직접적인 스킬 효과는 몬스터가 죽을때 제공

		break;
	case EPassiveItemType::AutomaticAttackDrone:
	{
		UWorld* World = GetWorld();
		if (!World || !DroneClass) return;

		APlayerCharacter* Player = Cast<APlayerCharacter>(Target);
		if (!Player) return;

		// 현재 플레이어가 가진 드론만 찾아서 세기
		TArray<AActor*> FoundDrones;
		UGameplayStatics::GetAllActorsOfClass(World, ADrone::StaticClass(), FoundDrones);

		TArray<ADrone*> OwnedDrones;
		for (AActor* Actor : FoundDrones)
		{
			ADrone* Drone = Cast<ADrone>(Actor);
			if (Drone && Drone->GetOwner() == Player)
			{
				OwnedDrones.Add(Drone);
			}
		}

		const int32 MaxDrones = 5;
		const int32 DesiredCount = FMath::Clamp(PassiveSkillData.StackCnt, 1, MaxDrones);
		const int32 CurrentCount = OwnedDrones.Num();

		UE_LOG(LogTemp, Warning, TEXT("[DroneSpawn] 현재 %d개, 목표 %d개"), CurrentCount, DesiredCount);

		for (int32 i = CurrentCount; i < DesiredCount; ++i)
		{
			FVector SpawnLoc = Player->GetActorLocation() + FVector(0, 0, 150);
			FRotator SpawnRot = FRotator::ZeroRotator;

			FActorSpawnParameters Params;
			Params.Owner = Player;
			Params.Instigator = Player;

			ADrone* Drone = World->SpawnActor<ADrone>(DroneClass, SpawnLoc, SpawnRot, Params);
			if (Drone)
			{
				Drone->SetOrbitTarget(Player);
				Drone->StartAngle = i * (360.f / DesiredCount);
				Drone->OrbitRadius = 200.f;
				Drone->OrbitSpeed = 60.f;

				UE_LOG(LogTemp, Warning, TEXT("[DroneSpawn] 드론 %d 생성, StartAngle=%.1f"), i, Drone->StartAngle);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[DroneSpawn] 드론 생성 실패!"));
			}
		}
		break;
	}
	default:
		break;
	}
	UE_LOG(LogTemp, Warning, TEXT("[PassiveSkill] %s 획득 스택: %d"),
		*PassiveSkillData.SkillName.ToString(), PassiveSkillData.StackCnt);

	UE_LOG(LogTemp, Warning, TEXT("향상된 능력치: AttackDamage=%.1f, AttackSpeed=%.1f, MaxHP=%.1f, MaxMP=%.1f, MoveSpeed=%.1f"),
		Stats->AttackDamage, Stats->AttackSpeed, Stats->MaxHP, Stats->MaxMP, Stats->MoveSpeed);
}