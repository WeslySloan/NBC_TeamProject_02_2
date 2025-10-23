// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GM_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class TEAM15CH3PROJECT_API AGM_GameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
    AGM_GameMode();

protected:
    virtual void BeginPlay() override;

private:
    //미션 타이머
    FTimerHandle MissionTimerHandle;

    //게임 오버 여부
    bool bIsGameOver;

    //미션 생성 시간
    float MissionSpawnDelay;

public:
    //게임 시작
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void StartGame();
    //게임 종료
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void EndGame();
    // 플레이어 리스폰
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void ResetGame();

    //웨이브 
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void StartNextWave();
    //적 스폰
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void SpawnEnemiesForWave();

    // 미션 생성
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void GenerateMission();
    //미션 성공
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void MissionSuccess();
    //미션 실패
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void MissionFail();

    //게임 클리어 조건
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void CheckWinCondition();
    //게임 오버 조건
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void CheckLoseCondition();


    //웨이브 단계 (점수) 출력
    UFUNCTION(BlueprintCallable, Category = "Player Stats")
    void PrintCurrentWave();
    //공격
    UFUNCTION(BlueprintCallable, Category = "Player Stats")
    void PlayerHit(float Damage);
};
