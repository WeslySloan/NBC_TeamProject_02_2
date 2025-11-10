#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NuclearAttack.generated.h"

UCLASS()
class TEAM15CH3PROJECT_API ANuclearAttack : public AActor
{
	GENERATED_BODY()
	
public:	
	ANuclearAttack();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NuclearAttack")
	USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NuclearAttack")
	UStaticMeshComponent* DroneMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NuclearAttack")
	UStaticMeshComponent* CastingMeshIn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NuclearAttack")
	UStaticMeshComponent* CastingMeshOut;

	UPROPERTY(EditAnywhere, Category = "Skill")
	float GrowSpeed;

	UPROPERTY(EditAnywhere, Category = "Skill")
	float MaxXScale;

	UPROPERTY(EditAnywhere, Category = "Skill")
	float BaseYScale;

	float CurrentXScale;

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


	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveSpeed = 1200.f;

	bool bIsMoving = true;

	FVector TargetLocation;

	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector MoveDirection = FVector::ForwardVector;
	void StartMoving();
	void StopMoving();

	// 살짝 진자운동 시켜줄 변수
	float TimeAccumulator;   
	float HoverSpeed;        
	float HoverAmplitude;

	bool bExploded = false;

	bool bShowDamageRadius = true;

	void Explode();

};
