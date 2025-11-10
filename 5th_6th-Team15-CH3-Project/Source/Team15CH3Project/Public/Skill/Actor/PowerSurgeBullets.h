#pragma once

#include "CoreMinimal.h"
#include "Skill/ActiveSkillItem.h"
#include "PowerSurgeBullets.generated.h"

class USphereComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;

UCLASS()
class TEAM15CH3PROJECT_API APowerSurgeBullets : public AActiveSkillItem
{
	GENERATED_BODY()
	
public:
	APowerSurgeBullets();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* ParticleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage; // 계산 필요함

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
protected:
	virtual void BeginPlay() override;

};
