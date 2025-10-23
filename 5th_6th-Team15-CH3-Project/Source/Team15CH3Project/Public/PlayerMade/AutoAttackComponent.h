// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h" // ?? UActorComponent 상속을 위해 필요
#include "AutoAttackComponent.generated.h"

// 전방 선언
class APlayerCharacter;
class UCharacterStatsComponent;
class APawn; // 몬스터 타겟 필터링을 위해 필요

// ?? 클래스 이름과 상속을 UActorComponent로 변경
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM15CH3PROJECT_API UAutoAttackComponent : public UActorComponent
{
    GENERATED_BODY()

public:    
    UAutoAttackComponent();

protected:
    // UActorComponent의 표준 함수 오버라이드
    virtual void BeginPlay() override;
    
public:
    // ?? BP에서 설정할 투사체 클래스
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
    TSubclassOf<AActor> ProjectileClass; 
    
    // ?? 몬스터 감지 범위
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
    float AttackRange = 1000.0f; 
    
    // ?? 몬스터 클래스 (BP_AIMonsterCharacter 필터링용)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
    TSubclassOf<APawn> MonsterClassFilter; // APawn으로 설정하여 BP_AIMonsterCharacter를 받습니다.

private:
    UPROPERTY()
    UCharacterStatsComponent* StatsComponent; 
    
    FTimerHandle AttackTimerHandle;

    // 공격 시작/제어 함수
    void StartAutoAttack();
    void FireProjectile();

    // 몬스터 감지 및 타겟 설정 함수
    APawn* FindTarget() const;
    
    // 발사 방향 결정 함수
    FRotator GetFireRotation(const APawn* Target) const;

    // 공격 속도에 따른 발사 주기 계산
    float CalculateAttackInterval() const;
};