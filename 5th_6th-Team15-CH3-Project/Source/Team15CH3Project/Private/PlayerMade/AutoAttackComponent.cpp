// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerMade/AutoAttackComponent.h"
#include "PlayerMade/PlayerCharacter.h"
#include "PlayerMade/CharacterStatsComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "PlayerMade/Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AI_Monster/AI_Monsters.h"

UAutoAttackComponent::UAutoAttackComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
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

void UAutoAttackComponent::StopAutoAttack()
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
        UE_LOG(LogTemp, Warning, TEXT("Auto Attack Stopped by PlayerIsDead event."));
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
                // 몬스터가 AAI_Monsters 타입이면 IsDead()로 상태 확인
                if (AAI_Monsters* Monster = Cast<AAI_Monsters>(Pawn))
                {
                    if (Monster->IsDead())
                    {
                        continue;
                    }
                }
                //else if (const UCharacterStatsComponent* EnemyStats = Pawn->FindComponentByClass<UCharacterStatsComponent>())
                //{
                //    if (EnemyStats->IsDead())
                //    {
                //        continue;
                //    }
                //}

                if (!Pawn->IsValidLowLevel() || Pawn->IsPendingKillPending())
                {
                    continue;
                }

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

    if (!Target)
        return;

    if (!Target->IsValidLowLevel() || Target->IsPendingKillPending())
        return;

    if (AAI_Monsters* Enemy = Cast<AAI_Monsters>(Target))
    {
        if (Enemy->IsDead())
        {
            UE_LOG(LogTemp, Warning, TEXT("[AutoAttack] Target is dead. Searching for next target..."));
            return; // 다음 Tick에서 새 타겟 자동 탐색
        }
    }
    else if (const UCharacterStatsComponent* EnemyStats = Target->FindComponentByClass<UCharacterStatsComponent>())
    {
        if (EnemyStats->IsDead())
        {
            UE_LOG(LogTemp, Warning, TEXT("[AutoAttack] Target's stats report dead. Searching for next target..."));
            return;
        }
    }

    // 투사체 발사
    FRotator BaseRotation = GetFireRotation(Target);
    int32 Count = StatsComponent->ProjectileCount;
    FVector SpawnLocation = GetOwner()->GetActorLocation() + FVector(0, 0, 50.0f);

    // 여러 발 발사를 위한 각도 계산
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

        APlayerCharacter* OwnerChar = Cast<APlayerCharacter>(GetOwner());
        FActorSpawnParameters Params;
        Params.Owner = OwnerChar;
        Params.Instigator = OwnerChar;

        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // 최초생성 충돌 방지용

        if (i == 0)
        {
            UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetOwner()->GetActorLocation());
        }

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
            UE_LOG(LogTemp, Warning, TEXT("Spawned Projectile is not of type AProjectile. Damage value not set."));
        }
    }
}

void UAutoAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!StatsComponent || !GetWorld())
        return;

    // 공격속도 변경 감지
    if (!FMath::IsNearlyEqual(StatsComponent->AttackSpeed, LastAttackSpeed))
    {
        LastAttackSpeed = StatsComponent->AttackSpeed;

        // 타이머 갱신
        GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
        StartAutoAttack();

        UE_LOG(LogTemp, Warning, TEXT("[AutoAttack] AttackSpeed changed → Timer restarted (%.2f)"), StatsComponent->AttackSpeed);
    }
}
