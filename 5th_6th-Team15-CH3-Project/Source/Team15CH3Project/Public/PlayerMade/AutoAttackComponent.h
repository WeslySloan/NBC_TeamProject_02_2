// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h" // ?? UActorComponent ����� ���� �ʿ�
#include "AutoAttackComponent.generated.h"

// ���� ����
class APlayerCharacter;
class UCharacterStatsComponent;
class APawn; // ���� Ÿ�� ���͸��� ���� �ʿ�

// ?? Ŭ���� �̸��� ����� UActorComponent�� ����
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM15CH3PROJECT_API UAutoAttackComponent : public UActorComponent
{
    GENERATED_BODY()

public:    
    UAutoAttackComponent();

protected:
    // UActorComponent�� ǥ�� �Լ� �������̵�
    virtual void BeginPlay() override;
    
public:
    // ?? BP���� ������ ����ü Ŭ����
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
    TSubclassOf<AActor> ProjectileClass; 
    
    // ?? ���� ���� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
    float AttackRange = 1000.0f; 
    
    // ?? ���� Ŭ���� (BP_AIMonsterCharacter ���͸���)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
    TSubclassOf<APawn> MonsterClassFilter; // APawn���� �����Ͽ� BP_AIMonsterCharacter�� �޽��ϴ�.

private:
    UPROPERTY()
    UCharacterStatsComponent* StatsComponent; 
    
    FTimerHandle AttackTimerHandle;

    // ���� ����/���� �Լ�
    void StartAutoAttack();
    void FireProjectile();

    // ���� ���� �� Ÿ�� ���� �Լ�
    APawn* FindTarget() const;
    
    // �߻� ���� ���� �Լ�
    FRotator GetFireRotation(const APawn* Target) const;

    // ���� �ӵ��� ���� �߻� �ֱ� ���
    float CalculateAttackInterval() const;
};