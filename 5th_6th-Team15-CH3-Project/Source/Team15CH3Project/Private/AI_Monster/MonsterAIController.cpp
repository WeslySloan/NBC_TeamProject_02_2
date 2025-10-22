#include "AI_Monster/MonsterAIController.h"
#include "NavigationSystem.h" //�׺���̼� ����� Ȱ���ϱ� ���ؼ� �ʿ��ϴ�
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"


AMonsterAIController::AMonsterAIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	//SetPerceptionComponent(*AIPerception); //�� ����� ��Ʈ�ѷ������� �ʿ��� ��� ������ ĳ���Ϳ��� �ʿ䰡 ����

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1500.0f; //���� �ϴ� �þ� �Ÿ�
	SightConfig->LoseSightRadius = 2000.0f; // ���� ����� ��ġ�� �Ÿ�
	SightConfig->PeripheralVisionAngleDegrees = 90.0f; //���� ���� ����
	SightConfig->SetMaxAge(5.0f); //���� �ϰ� ��Դ� �ð� ��

	SightConfig->DetectionByAffiliation.bDetectEnemies = true; //�� �ν�
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true; // ���� �ν�
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true; // �߸� �ν�

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

	GetWorldTimerManager().SetTimer(RandomMoveTimer, this, &AMonsterAIController::MoveTorRandomLocation, 3.0f, true, 1.0f);//this <- AI��Ʈ�ѷ� �ڱ� �ڽ�
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
		MyPawn->GetActorLocation(), //���� ĳ���� ��ġ
		MoveRadius, //Ȱ�� �ݰ�
		RandomLocation //���� ��� ���� �����ϴ� ��
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
		StartChasing(Actor); //�÷��̾� �߰��ϱ�
	}
	else
	{
		StopChasing(); //�÷��̾� �߰� ���ϰ� ��ȸ�ϱ�
	}
}

void AMonsterAIController::StartChasing(AActor* Target)
{
	if (bIsChasing && CurrentTarget == Target) return;

	CurrentTarget = Target;
	bIsChasing = true;

	GetWorldTimerManager().ClearTimer(RandomMoveTimer);

	//if(AAIMonsterCharacter* )

}
void AMonsterAIController::StopChasing()
{

}
void AMonsterAIController::UpdateChase() 
{

}