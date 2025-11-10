#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossMonster.generated.h"


UCLASS()
class TEAM15CH3PROJECT_API ABossMonster : public ACharacter
{
	GENERATED_BODY()

public:
	ABossMonster();

	bool CanAttack(APawn* Target) const;
	void PerformAttack(APawn* Target);
	bool IsDead() const { return CurrentHP <= 0.f; }
	
	
	// === 이동 ===
	UPROPERTY(EditAnywhere, Category = "AI")
	float WalkSpeed = 500.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float RunSpeed = 560.0f;

	// === 보상(골드 고정 드랍) ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	int32 GoldReward = 500;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") float AttackCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") float AttackDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") float AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") float MaxHP;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat") float CurrentHP;

private:

	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

	//void HandleDeath();
	void SetMovementSpeed(float NewSpeed);

	float LastAttackTime = -1000.0f;
};
