#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class TEAM15CH3PROJECT_API AProjectile : public AActor
{
    GENERATED_BODY()

public:
    AProjectile();

    // 💡 투사체의 충돌 및 루트 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* CollisionComp;

    // 💡 투사체 움직임 관리 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UProjectileMovementComponent* ProjectileMovement;

    // 💡 AutoAttackComponent에서 전달받을 데미지 변수 (BP에서도 접근 가능)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float Damage = 0.0f;

protected:
    virtual void BeginPlay() override;

    // 💡 충돌 발생 시 호출될 함수 (Overlap이 아닌 Hit을 사용하면 물리적인 상호작용 가능)
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
    // 💡 투사체 초기화 함수 (AutoAttackComponent에서 호출)
    void InitializeProjectile(float InitialDamage);
};