#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AI_Monsters.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UBIackboardComponent;
class UCapsuleComponent;

UCLASS()
class TEAM15CH3PROJECT_API AAI_Monsters : public ACharacter
{
	GENERATED_BODY()

public:

	AAI_Monsters();
	void StartBehaviorTree();
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const;


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


private:
	void SetMovemonetSpeed(float NewSpeed);

	UPROPERTY(EditAnyWhere, Category = "AI")
	float WalkSpeed = 500.0f; //기본속도가 600으로 설정

	UPROPERTY(EditAnyWhere, Category = "AI")
	float RunSpeed = 700.0f;

};
