 #pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "MonsterAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;

UCLASS()
class TEAM15CH3PROJECT_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMonsterAIController();



protected:
	virtual void OnPossess(APawn* InPawn) override; //OnPossess 1순위 호출 2순위 BeginPlay
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerception;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAISenseConfig_Sight* SightConfig;

	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:
	void MoveTorRandomLocation();
	FTimerHandle RandomMoveTimer;

	UPROPERTY(EditAnywhere, Category = "AI")
	float MoveRadius = 1000.0f;

	UPROPERTY()
	AActor* CurrentTarget = nullptr;

	bool bIsChasing = false;
	FTimerHandle ChaseTimer;

	void StartChasing(AActor* Target);
	void StopChasing();
	void UpdateChase();
};
