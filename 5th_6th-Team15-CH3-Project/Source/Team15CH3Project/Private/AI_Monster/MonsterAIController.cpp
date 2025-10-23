#include "AI_Monster/MonsterAIController.h"
#include "NavigationSystem.h" //네비게이션 기능을 활용하기 위해서 필요하다
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "AI_Monster/AIMonsterCharacter.h"


AMonsterAIController::AMonsterAIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	//SetPerceptionComponent(*AIPerception); //이 기능은 컨트롤러에서는 필요한 기능 하지만 캐릭터에는 필요가 없음

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1500.0f; //감지 하는 시야 거리
	SightConfig->LoseSightRadius = 2000.0f; // 감지 대상을 놓치는 거리
	SightConfig->PeripheralVisionAngleDegrees = 90.0f; //감지 각도 범위
	SightConfig->SetMaxAge(5.0f); //감지 하고 까먹는 시간 초

	SightConfig->DetectionByAffiliation.bDetectEnemies = true; //적 인식
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true; // 팀원 인식
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true; // 중립 인식

	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());
}

void AMonsterAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIPerception)
	{
		AIPerception->OnTargetPerceptionUpdated.AddDynamic
		(
			this,
			&AMonsterAIController::OnPerceptionUpdated
		);
	}

	GetWorldTimerManager().SetTimer(RandomMoveTimer, this, &AMonsterAIController::MoveTorRandomLocation, 3.0f, true, 1.0f);//this <- AI컨트롤러 자기 자신
}

void AMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (InPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Monster] AI Controller is controlling %s."), *InPawn->GetName());
	}
}

void AMonsterAIController::MoveTorRandomLocation()
{
	APawn* MyPawn = GetPawn();
	if (!MyPawn)
	{
		return;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSystem)
	{
		return;
	}

	FNavLocation RandomLocation;

	bool bFouindLocation = NavSystem->GetRandomReachablePointInRadius
	(
		MyPawn->GetActorLocation(), //현제 캐릭터 위치
		MoveRadius, //활동 반경
		RandomLocation //계산된 결과 값을 저장하는 곳
	);

	if (bFouindLocation)
	{
		MoveToLocation(RandomLocation.Location);

		UE_LOG(LogTemp, Warning, TEXT("[Monster] Move target: %s"), *RandomLocation.Location.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[Monster] not find a location"));
	}
}

void AMonsterAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (Actor != PlayerPawn)
	{
		return;
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		StartChasing(Actor); //플레이어 추격하기
	}
	else
	{
		StopChasing(); //플레이어 추격 안하고 배회하기
	}
}

void AMonsterAIController::StartChasing(AActor* Target)
{
	if (bIsChasing && CurrentTarget == Target) return;

	CurrentTarget = Target;
	bIsChasing = true;

	GetWorldTimerManager().ClearTimer(RandomMoveTimer);

	if (AAIMonsterCharacter* EnemyChar = Cast<AAIMonsterCharacter>(GetPawn()))
	{
		EnemyChar->SetMovemonetSpeed(EnemyChar->RunSpeed);
	}
	UpdateChase();

	GetWorldTimerManager().SetTimer
	(
		ChaseTimer,
		this,
		&AMonsterAIController::UpdateChase,
		0.25f,
		true
	);

}
void AMonsterAIController::StopChasing()
{
	if (!bIsChasing) return;
	CurrentTarget = nullptr;
	bIsChasing = false;
	GetWorldTimerManager().ClearTimer(ChaseTimer);
	StopMovement();

	if (AAIMonsterCharacter* EnemyChar = Cast<AAIMonsterCharacter>(GetPawn()))
	{
		EnemyChar->SetMovemonetSpeed(EnemyChar->WalkSpeed);
	}

	GetWorldTimerManager().SetTimer
	(
		ChaseTimer,
		this,
		&AMonsterAIController::MoveTorRandomLocation,
		3.0f,
		true,
		2.0f
	);
}
void AMonsterAIController::UpdateChase() 
{
	if (CurrentTarget && bIsChasing)
	{
		MoveToActor(CurrentTarget, 100.0f);
	}
}