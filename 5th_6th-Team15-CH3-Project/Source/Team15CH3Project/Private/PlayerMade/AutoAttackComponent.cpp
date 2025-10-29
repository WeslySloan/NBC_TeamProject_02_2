// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMade/AutoAttackComponent.h"
#include "PlayerMade/PlayerCharacter.h"
#include "PlayerMade/CharacterStatsComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "PlayerMade/Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

UAutoAttackComponent::UAutoAttackComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UAutoAttackComponent::BeginPlay()
{
    Super::BeginPlay();

    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if (OwnerPawn)
    {
        StatsComponent = OwnerPawn->FindComponentByClass<UCharacterStatsComponent>();
    }

    if (StatsComponent && ProjectileClass)
    {
        StartAutoAttack();
    }
}

// ====================================================================
// 공격 제어 로직
// ====================================================================

void UAutoAttackComponent::StartAutoAttack()
{
    float AttackInterval = CalculateAttackInterval();

    GetWorld()->GetTimerManager().SetTimer(
        AttackTimerHandle,
        this,
        &UAutoAttackComponent::FireProjectile,
        AttackInterval,
        true
    );
}

float UAutoAttackComponent::CalculateAttackInterval() const
{
    if (StatsComponent && StatsComponent->AttackSpeed > 0)
    {
        return 1.0f / StatsComponent->AttackSpeed;
    }
    return 1.0f;
}

// ====================================================================
// 타겟 및 발사 로직
// ====================================================================

APawn* UAutoAttackComponent::FindTarget() const
{
    UWorld* World = GetWorld();
    APlayerCharacter* OwnerChar = Cast<APlayerCharacter>(GetOwner());
    if (!World || !OwnerChar || !MonsterClassFilter) return nullptr;

    TArray<AActor*> OverlappingActors;
    UGameplayStatics::GetAllActorsOfClass(World, MonsterClassFilter, OverlappingActors);

    if (OverlappingActors.Num() > 0)
    {
        APawn* BestTarget = nullptr;
        float ClosestDistSq = FMath::Square(AttackRange);

        FVector OwnerLocation = OwnerChar->GetActorLocation();

        for (AActor* Actor : OverlappingActors)
        {
            if (APawn* Pawn = Cast<APawn>(Actor))
            {
                float DistSq = FVector::DistSquared(OwnerLocation, Pawn->GetActorLocation());

                if (DistSq <= ClosestDistSq)
                {
                    ClosestDistSq = DistSq;
                    BestTarget = Pawn;
                }
            }
        }
        return BestTarget;
    }

    return nullptr;
}

FRotator UAutoAttackComponent::GetFireRotation(const APawn* Target) const
{
    if (Target)
    {
        FVector Direction = Target->GetActorLocation() - GetOwner()->GetActorLocation();
        return Direction.Rotation();
    }
    else
    {
        return GetOwner()->GetActorRotation();
    }
}

void UAutoAttackComponent::FireProjectile()
{
    if (!ProjectileClass || !StatsComponent) return;

    UWorld* World = GetWorld();
    if (!World) return;

    APawn* Target = FindTarget();
    FRotator BaseRotation = GetFireRotation(Target);
    int32 Count = StatsComponent->ProjectileCount; // 1. 스탯에서 Count를 가져옵니다.
    FVector SpawnLocation = GetOwner()->GetActorLocation() + FVector(0, 0, 50.0f);

    // 2. 여러 발 발사를 위한 각도 계산
    float SpreadAngle = 10.0f; // 총알 사이의 각도 (10도)
    float HalfAngle = SpreadAngle * (Count - 1) / 2.0f; // 전체 각도의 절반

    for (int32 i = 0; i < Count; ++i)
    {
        FRotator FinalRotation = BaseRotation;

        if (Count > 1)
        {
            float CurrentAngle = -HalfAngle + (i * SpreadAngle);
            FinalRotation.Yaw += CurrentAngle;
        }

        FActorSpawnParameters Params;
        Params.Owner = GetOwner();
        Params.Instigator = GetOwner()->GetInstigator();

        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // 최초생성 충돌 방지용

        // 투사체 생성
        AActor* NewActor = World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, FinalRotation, Params);

        if (AProjectile* Projectile = Cast<AProjectile>(NewActor))
        {
            if (Projectile->ProjectileMovement)
            {
                Projectile->ProjectileMovement->Velocity = FinalRotation.Vector() * Projectile->ProjectileMovement->InitialSpeed;
            }
            Projectile->InitializeProjectile(StatsComponent->AttackDamage);
            UE_LOG(LogTemp, Warning, TEXT("Projectile on Spawn test"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Spawned Projectile is not of type AProjectile. Damage value not set."));
        }
    }
}