#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI_Monster/MonsterBullet.h"
#include "Ranged_MonsterController.generated.h"

class UCapsuleComponent;

UCLASS()
class TEAM15CH3PROJECT_API ARanged_MonsterController : public AAIController
{
	GENERATED_BODY()

public:
	ARanged_MonsterController();

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	// 근거리 컨트롤러랑 똑같이 이름 맞춤
	void StartChaseLoop();
	void UpdateChaseLoop();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackRange = 800.f;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UCapsuleComponent* CapsuleComp;

private:
	UPROPERTY()
	APawn* PlayerPawnCached = nullptr;

	FTimerHandle ChaseTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Chase")
	float MoveUpdateInterval = 0.25f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float MinKeepDistance = 300.f;

	UPROPERTY(EditAnywhere, Category = "Chase")
	float AcceptanceRadius = 200.f;
};
