#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "AI_MonsterController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UBlackboardComponent; 
class UCapsuleComponent;

UCLASS()
class TEAM15CH3PROJECT_API AAI_MonsterController : public AAIController
{
	GENERATED_BODY()

public:
	AAI_MonsterController();
	virtual void OnPossess(APawn* InPawn) override; //OnPossess 1순위 호출 2순위 BeginPlay

	void StartChaseLoop();
	void UpdateChaseLoop();


protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UCapsuleComponent* CapsuleComp;

	UPROPERTY()
	AActor* CurrentTarget = nullptr;

private:

	AActor* TargetActor;

	FTimerHandle ChaseTimerHandle;                                   
	UPROPERTY(EditAnywhere, Category = "Chase") float MoveUpdateInterval = 0.25f; 
	UPROPERTY(EditAnywhere, Category = "Combat") float AttackRange = 180.f;
	UPROPERTY() APawn* PlayerPawnCached = nullptr;                               

	                                        
};
