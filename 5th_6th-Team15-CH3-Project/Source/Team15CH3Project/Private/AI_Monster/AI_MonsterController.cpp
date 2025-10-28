#include "AI_Monster/AI_MonsterController.h"
#include "AI_Monster/AIMonsterCharacter.h"
#include "AI_Monster/MonsterAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TimerManager.h"
#include "AI_Monster/AI_Monsters.h"

AAI_MonsterController::AAI_MonsterController()
{

}

void AAI_MonsterController::BeginPlay()
{
	Super::BeginPlay();
	PlayerPawnCached = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	//StartChaseLoop();
}

void AAI_MonsterController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (InPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Monster] AI Controller is controlling %s."), *InPawn->GetName());
	}
}

void AAI_MonsterController::StartChaseLoop()                  
{
	UE_LOG(LogTemp, Warning, TEXT("[RUN] AI Controller is controlling"));
	MoveToActor(PlayerPawnCached, 120.f, true);
	UE_LOG(LogTemp, Warning, TEXT("[PlayerPawnCash] %s"), *(PlayerPawnCached->GetName()));//AcceptanceRadius= 120πÃ≈Õ  bStopOnOverlap=∞° true
	if (!GetWorldTimerManager().IsTimerActive(ChaseTimerHandle)) 
	{
		GetWorldTimerManager().SetTimer(                       
			ChaseTimerHandle, this, &AAI_MonsterController::UpdateChaseLoop,
			 0.5f ,true);

	}
	
}                                                              

void AAI_MonsterController::UpdateChaseLoop()
{
	UE_LOG(LogTemp, Warning, TEXT("[UPDateLoop] AI Controller is controlling"));
	APawn* SelfPawn = GetPawn();
	if (!SelfPawn || !PlayerPawnCached) { return; }

	/*if (const AAI_Monsters* M = Cast<AAI_Monsters>(SelfPawn))
	{
		if (M->IsDead())
		{
			StopMovement();
			GetWorldTimerManager().ClearTimer(ChaseTimerHandle);
			return;
		}
	}*/

	const float Dist2D = FVector::Dist2D(SelfPawn->GetActorLocation(), PlayerPawnCached->GetActorLocation());


	if (Dist2D <= AttackRange)
	{
		StopMovement();
		if (AAI_Monsters* M = Cast<AAI_Monsters>(SelfPawn))
		{
			if (M->CanAttack(PlayerPawnCached))
			{
				M->PerformAttack(PlayerPawnCached);
			}
		}
		return;
	}


	MoveToActor(PlayerPawnCached, 120.f, true);
}