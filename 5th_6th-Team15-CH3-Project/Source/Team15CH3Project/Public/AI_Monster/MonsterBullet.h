#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MonsterBullet.generated.h"

class UDamageType;

UCLASS()
class TEAM15CH3PROJECT_API AMonsterBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	AMonsterBullet();

	UPROPERTY(VisibleAnywhere, Category = "Bullet")
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, Category = "Bullet")
	UProjectileMovementComponent* ProjecttileMovementComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
	TSubclassOf<UDamageType> DamageTypeClass = UDamageType::StaticClass();


	void FireInDirection(const FVector& ShootDirection);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Damage = 15.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
	float InitialSpeed = 3000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
	float MaxSpeed = 3000.f;

protected:
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

private:
	void ApplyDamageAndDestroy(AActor* HitActor);
};
