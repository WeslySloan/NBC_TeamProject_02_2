#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NapalmAttack.generated.h"

class USphereComponent;

UCLASS()
class TEAM15CH3PROJECT_API ANapalmAttack : public AActor
{
	GENERATED_BODY()
	
public:	
	ANapalmAttack();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// 备己夸家
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NapalmAttack")
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NapalmAttack")
	USphereComponent* CollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NapalmAttack")
	UStaticMeshComponent* CastingMeshIn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NapalmAttack")
	UStaticMeshComponent* CastingMeshOut;

	UPROPERTY(EditAnywhere, Category = "Skill")
	float GrowSpeed;

	UPROPERTY(EditAnywhere, Category = "Skill")
	float MaxXScale;

	UPROPERTY(EditAnywhere, Category = "Skill")
	float BaseYScale;

	float CurrentXScale;

	// 气惯 包访
	UPROPERTY(EditAnywhere, Category = "Skill")
	float DamageRadius;

	UPROPERTY(EditAnywhere, Category = "Skill")
	float Damage;

	UPROPERTY(EditAnywhere, Category = "Explosion")
	int32 MinExplosionCount;

	UPROPERTY(EditAnywhere, Category = "Explosion")
	int32 MaxExplosionCount;

	UPROPERTY(EditAnywhere, Category = "Explosion")
	float MinExplosionDelay;

	UPROPERTY(EditAnywhere, Category = "Explosion")
	float MaxExplosionDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Effect")
	float EllipseRadiusX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Effect")
	float EllipseRadiusY;

	TArray<FTimerHandle> ExplosionTimers;

	UPROPERTY(EditAnywhere, Category = "Skill")
	UParticleSystem* ExplosionEffect;

	bool bExploded = false;

	void Explode();
};
