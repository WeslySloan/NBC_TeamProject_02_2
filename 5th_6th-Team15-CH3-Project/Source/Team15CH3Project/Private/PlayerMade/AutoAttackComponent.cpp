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
        // Owner���� StatsComponent�� ã���ϴ�.
        StatsComponent = OwnerPawn->FindComponentByClass<UCharacterStatsComponent>();
    }

    // ���� �ӵ��� ���� �ڵ� ���� Ÿ�̸� ����
    if (StatsComponent && ProjectileClass)
    {
        StartAutoAttack();
    }
}

// ====================================================================
// ���� ���� ����
// ====================================================================

void UAutoAttackComponent::StartAutoAttack()
{
    float AttackInterval = CalculateAttackInterval();

    GetWorld()->GetTimerManager().SetTimer(
        AttackTimerHandle,
        this,
        &UAutoAttackComponent::FireProjectile,
        AttackInterval,
        true // �ݺ� ����
    );
}

float UAutoAttackComponent::CalculateAttackInterval() const
{
    if (StatsComponent && StatsComponent->AttackSpeed > 0)
    {
        // ���� �ӵ�(AttackSpeed)�� �ʴ� ���� Ƚ���̹Ƿ�, �ֱ�� 1 / AttackSpeed �Դϴ�.
        return 1.0f / StatsComponent->AttackSpeed;
    }
    return 1.0f; // �⺻ �ֱ� 1��
}

// ====================================================================
// Ÿ�� �� �߻� ����
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

    // ���� ����
    APawn* Target = FindTarget();

    // Ÿ�ٿ� ���� �߻� ���� ����
    FRotator BaseRotation = GetFireRotation(Target);

    int32 Count = StatsComponent->ProjectileCount; // ź �߻緮 �ݿ�

    FVector SpawnLocation = GetOwner()->GetActorLocation() + FVector(0, 0, 50.0f);

    // ProjectileCount�� ���� �߻� ���� ���
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

        // ����ü ����
        AActor* NewProjectile = World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, FinalRotation, Params);

        // ������ �̺�Ʈ (�ӽ� ����)
        if (NewProjectile)
        {
            UE_LOG(LogTemp, Warning, TEXT("Fired Projectile with Damage: %f"), StatsComponent->AttackDamage);
        }
    }
}