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
        // Owner에서 StatsComponent를 찾습니다.
        StatsComponent = OwnerPawn->FindComponentByClass<UCharacterStatsComponent>();
    }

    // 공격 속도에 따라 자동 공격 타이머 시작
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
        true // 반복 설정
    );
}

float UAutoAttackComponent::CalculateAttackInterval() const
{
    if (StatsComponent && StatsComponent->AttackSpeed > 0)
    {
        // 공격 속도(AttackSpeed)는 초당 공격 횟수 // 주기는 1 / AttackSpeed 
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
        // 몬스터 중 가장 가까운 타겟을 찾는 로직...
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

    int32 Count = StatsComponent->ProjectileCount;

    FVector SpawnLocation = GetOwner()->GetActorLocation() + FVector(0, 0, 50.0f);

    // ProjectileCount에 따른 발사 각도 계산
    float SpreadAngle = 10.0f;
    float HalfAngle = SpreadAngle * (Count - 1) / 2.0f;

    for (int32 i = 0; i < Count; ++i)
    {
        FRotator FinalRotation = BaseRotation;

        if (Count > 1)
        {
            float CurrentAngle = HalfAngle - (i * SpreadAngle);
            FinalRotation.Yaw += CurrentAngle;
        }

        FActorSpawnParameters Params;
        Params.Owner = GetOwner();
        Params.Instigator = GetOwner()->GetInstigator();

        // 투사체 생성
        AActor* NewActor = World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, FinalRotation, Params);

        if (AProjectile* Projectile = Cast<AProjectile>(NewActor))
        {
            if (Projectile->ProjectileMovement)
            {
                Projectile->ProjectileMovement->Velocity = FinalRotation.Vector() * Projectile->ProjectileMovement->InitialSpeed;
            }
            Projectile->InitializeProjectile(StatsComponent->AttackDamage);
        }
        else
        {
            // ProjectileClass에 AProjectile이 아닌 다른 Actor BP가 할당된 경우 대비
            UE_LOG(LogTemp, Warning, TEXT("Spawned Projectile is not of type AProjectile. Damage value not set."));
        }
    }
}