// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMade/AutoAttackComponent.h"
#include "PlayerMade/PlayerCharacter.h"
#include "PlayerMade/CharacterStatsComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

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

    // 몬스터 감지
    APawn* Target = FindTarget();

    // 타겟에 따라 발사 방향 결정
    FRotator BaseRotation = GetFireRotation(Target);

    int32 Count = StatsComponent->ProjectileCount; // 탄 발사량 반영

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

        // 투사체 생성
        AActor* NewProjectile = World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, FinalRotation, Params);

        // 데미지 이벤트 (임시)
        if (NewProjectile)
        {
            UE_LOG(LogTemp, Warning, TEXT("Fired Projectile with Damage: %f"), StatsComponent->AttackDamage);
        }
    }
}