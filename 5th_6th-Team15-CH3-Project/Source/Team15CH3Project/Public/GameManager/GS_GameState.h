// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GS_GameState.generated.h"

UCLASS()
class TEAM15CH3PROJECT_API AGS_GameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AGS_GameState();

    //플레이어 체력
    UPROPERTY(BlueprintReadWrite, Replicated, Category = "Player Stats")
    float Stamina;

    //현재 웨이브
    UPROPERTY(BlueprintReadWrite, Replicated, Category = "Wave Stats")
    int32 CurrentWave;

    //보스 처치 여부
    UPROPERTY(BlueprintReadWrite, Replicated, Category = "Wave Stats")
    bool bBossDefeated;

    //미션 실패 여부
    UPROPERTY(BlueprintReadWrite, Replicated, Category = "Mission")
    bool bMissionFailed;

    //적 처치 수
    UPROPERTY(BlueprintReadWrite, Replicated, Category = "Player Stats")
    int32 EnemyKillCount;

private:
    // 플레이어 체력 감소
    void ModifyPlayerHealth(float Amount);

    // 적 처치 증가
    void AddEnemyKillCount();
};
