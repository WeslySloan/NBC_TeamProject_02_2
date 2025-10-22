#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionTypes.h"
#include "AIMonsterCharacter.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;

UCLASS()
class TEAM15CH3PROJECT_API AAIMonsterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AAIMonsterCharacter();

	void SetMovemonetSpeed(float NewSpeed);

	UPROPERTY (EditAnyWhere, Category = "AI")
	float WalkSpeed = 500.0f; //기본속도가 600으로 설정

	UPROPERTY(EditAnyWhere, Category = "AI")
	float RunSpeed = 700.0f; 

protected:
	virtual void BeginPlay() override;

};
