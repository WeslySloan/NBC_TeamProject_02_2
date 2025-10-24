// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMade/Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AProjectile::AProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    RootComponent = CollisionComp;

    CollisionComp->InitSphereRadius(15.0f);

    CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionComp->SetCollisionProfileName(TEXT("BlockAll"));
    CollisionComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 3000.0f;
    ProjectileMovement->MaxSpeed = 3000.0f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;

    InitialLifeSpan = 1.0f;
}

void AProjectile::BeginPlay()
{
    Super::BeginPlay();

    CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this && OtherActor != GetOwner())
    {
        if (OtherActor->IsA(APawn::StaticClass()))
        {
            UE_LOG(LogTemp, Warning, TEXT("Projectile hit Monster: %s. Damage: %f"), *OtherActor->GetName(), Damage);

            UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, NULL);
        }

        // 충돌 후 파괴
        Destroy();
    }
}

void AProjectile::InitializeProjectile(float InitialDamage)
{
    Damage = InitialDamage;
}