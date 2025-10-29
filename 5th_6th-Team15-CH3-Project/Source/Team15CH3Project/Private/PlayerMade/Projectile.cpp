// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMade/Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AI_Monster/AI_Monsters.h"

AProjectile::AProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    SetLifeSpan(5.0f);

    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    RootComponent = CollisionComp;

    CollisionComp->InitSphereRadius(10.0f);

    CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 3000.0f;
    ProjectileMovement->MaxSpeed = 3000.0f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;
}

void AProjectile::BeginPlay()
{
    Super::BeginPlay();

    /// 추가됨
    CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    // 스폰 후 잠시 후에 활성화
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, [this]()
        {
            if (CollisionComp)
                CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // Overlap 전용 활성화
        }, 0.05f, false);
    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);
    /// 

}

// Overlap 이벤트 처리 함수 구현
void AProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 자신을 발사한 캐릭터(Owner)와 충돌하는 것은 무시
    if (OtherActor && OtherActor != this && OtherActor != GetOwner())
    {
        if (OtherActor->IsA(AAI_Monsters::StaticClass()))
        {
            // 이 조건문을 통과했으므로 몬스터에게 데미지를 적용합니다.
            UE_LOG(LogTemp, Warning, TEXT("Projectile Overlapped Monster: %s. Damage: %f"), *OtherActor->GetName(), Damage);

            // 데미지 적용
            UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, NULL);
        }

        UE_LOG(LogTemp, Warning, TEXT("Test2"));

        // Overlap 발생 후 파괴
        Destroy();
    }
}

void AProjectile::InitializeProjectile(float InitialDamage)
{
    Damage = InitialDamage;
    // 발사 방향 설정은 AutoAttackComponent에서 ProjectileMovement::Velocity를 직접 설정합니다.
}


//void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//    if (OtherActor && OtherActor != this && OtherActor != GetOwner())
//    {
//        if (OtherActor->IsA(APawn::StaticClass()))
//        {
//            UE_LOG(LogTemp, Warning, TEXT("Projectile hit Monster: %s. Damage: %f"), *OtherActor->GetName(), Damage);
//            UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, NULL);
//        }
//        UE_LOG(LogTemp, Warning, TEXT("Test2"));
//        // 충돌 후 파괴
//        Destroy();
//    }
//    else 
//    {
//        // OtherActor && OtherActor != this && OtherActor != GetOwner()
//        // 타이머 걸어서 Destory 시키기
//
//        // 이걸로 투사체 안없어지면 else if로 삭제시키기
//    }
//}