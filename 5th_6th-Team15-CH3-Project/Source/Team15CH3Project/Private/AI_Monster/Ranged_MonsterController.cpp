#include "AI_Monster/Ranged_MonsterController.h"
#include "AI_Monster/AI_Monsters.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"



ARanged_MonsterController::ARanged_MonsterController()
{

}

void ARanged_MonsterController::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawnCached = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void ARanged_MonsterController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (InPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("[RangedMonster] AI Controller is controlling %s."), *InPawn->GetName());
	}

	if (PlayerPawnCached)
	{
		StartChaseLoop();
	}
}

void ARanged_MonsterController::StartChaseLoop()
{
	if (!GetWorldTimerManager().IsTimerActive(ChaseTimerHandle))
	{
		GetWorldTimerManager().SetTimer(
			ChaseTimerHandle,
			this,
			&ARanged_MonsterController::UpdateChaseLoop,
			MoveUpdateInterval,
			true
		);
	}

	if (PlayerPawnCached)
	{
		MoveToActor(PlayerPawnCached, AcceptanceRadius, true);
	}
}

void ARanged_MonsterController::UpdateChaseLoop()
{
	APawn* SelfPawn = GetPawn();
	if (!SelfPawn || !PlayerPawnCached)
	{
		return;
	}

	if (const AAI_Monsters* Monster = Cast<AAI_Monsters>(SelfPawn))
	{
		if (Monster->IsDead())
		{
			StopMovement();
			GetWorldTimerManager().ClearTimer(ChaseTimerHandle);
			return;
		}
	}

	const float Dist2D = FVector::Dist2D(SelfPawn->GetActorLocation(), PlayerPawnCached->GetActorLocation());


	if (Dist2D <= AttackRange)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Do Not Move]"));
		StopMovement();

		if (AAI_Monsters* Monster = Cast<AAI_Monsters>(SelfPawn))
		{
			UE_LOG(LogTemp, Warning, TEXT("[It's me]"));
			if (Monster->ReangCanAttak(PlayerPawnCached))
			{
				/*const FVector ToTarget = PlayerPawnCached->GetActorLocation() - Monster->GetActorLocation();
				const FRotator LookRot = ToTarget.Rotation();
				Monster->SetActorRotation(FRotator(0.f, LookRot.Yaw, 0.f));*/
				
				UE_LOG(LogTemp, Warning, TEXT("[RangedMonster] Shoot!"));
				Monster->BulletAttack(PlayerPawnCached);
			}
		}
		return;
	}

	MoveToActor(PlayerPawnCached, 750.0f, true);
}