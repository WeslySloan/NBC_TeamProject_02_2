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

    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);

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

    if (!GetOwner() && GetInstigator())
    {
        SetOwner(GetInstigator());
    }
}

// Overlap 이벤트 처리 함수 구현
void AProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 자신을 발사한 캐릭터(Owner)와 충돌하는 것은 무시
    if (OtherActor && OtherActor != this && OtherActor != GetOwner())
    {
        // OtherActor가 다른 AProjectile 인스턴스라면 Overlap 처리를 무시하고 즉시 종료합니다.
        if (OtherActor->IsA(AProjectile::StaticClass()))
        {
            return;
        }

        if (OtherActor->IsA(AAI_Monsters::StaticClass()))
        {
            // 이 조건문을 통과했으므로 몬스터에게 데미지를 적용합니다.
            UE_LOG(LogTemp, Warning, TEXT("Projectile Overlapped Monster: %s. Damage: %f"), *OtherActor->GetName(), Damage);

            // 데미지 적용
            AController* InstigatorController = GetInstigatorController();
            UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, NULL);
        }

        // Overlap 발생 후 파괴 (다른 모든 대상과의 충돌 시 파괴)
        Destroy();
    }
}

void AProjectile::InitializeProjectile(float InitialDamage)
{
    Damage = InitialDamage;
}