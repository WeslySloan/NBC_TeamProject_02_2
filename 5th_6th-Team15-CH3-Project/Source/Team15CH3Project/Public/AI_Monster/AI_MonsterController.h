// Fill out your copyright notice in the Description page of Project Settings.

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
	void StartBehaviorTree();
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const;

	virtual void OnPossess(APawn* InPawn) override; //OnPossess 1순위 호출 2순위 BeginPlay
	void SetMovemonetSpeed(float NewSpeed);

	UPROPERTY(EditAnyWhere, Category = "AI")
	float WalkSpeed = 500.0f; //기본속도가 600으로 설정

	UPROPERTY(EditAnyWhere, Category = "AI")
	float RunSpeed = 700.0f;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerception;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UBlackboardComponent* BlackboardComp;

	UPROPERTY()
	AActor* CurrentTarget = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTreeAsset;

	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:

	AActor* TargetActor;
};
