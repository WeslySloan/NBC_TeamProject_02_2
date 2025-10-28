#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AI_Monsters.generated.h"

class UCapsuleComponent;

UCLASS()
class TEAM15CH3PROJECT_API AAI_Monsters : public ACharacter
{
	GENERATED_BODY()

public:

	AAI_Monsters();

	bool CanAttack(APawn* Target) const;     
	void PerformAttack(APawn* Target);       
	bool IsDead() const { return CurrentHP <= 0.f; } 

	UPROPERTY(EditAnyWhere, Category = "AI")
	float WalkSpeed = 400.0f; // AI 몬스터 속도값을 정해주는 기능.

	UPROPERTY(EditAnyWhere, Category = "AI")
	float RunSpeed = 500.0f;


protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UCapsuleComponent* CapsuleComp;

	UPROPERTY()
	AActor* CurrentTarget = nullptr;

private:

	UPROPERTY(EditAnywhere, Category = "Combat") float AttackCooldown; 
	UPROPERTY(EditAnywhere, Category = "Combat") float AttackDamage; 
	UPROPERTY(EditAnywhere, Category = "Combat") float AttackRange; 
	UPROPERTY(EditAnywhere, Category = "Combat") float MaxHP;  
	UPROPERTY(VisibleAnywhere, Category = "Combat") float CurrentHP;  

	UPROPERTY(EditDefaultsOnly, Category = "AI|Combat")
	class UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Combat")
	class USoundBase* AttackSound;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Combat")
	class UParticleSystem* HitEffect;

	//virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	float LastAttackTime = -1000.f;          

	void SetMovementSpeed(float NewSpeed);

};
