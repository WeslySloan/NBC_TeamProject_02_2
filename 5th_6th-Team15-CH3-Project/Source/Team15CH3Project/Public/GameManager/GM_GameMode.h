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
    //�̼� Ÿ�̸�
    FTimerHandle MissionTimerHandle;

    //���� ���� ����
    bool bIsGameOver;

    //�̼� ���� �ð�
    float MissionSpawnDelay;

public:
    //���� ����
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void StartGame();
    //���� ����
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void EndGame();
    // �÷��̾� ������
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void ResetGame();

    //���̺� 
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void StartNextWave();
    //�� ����
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void SpawnEnemiesForWave();

    // �̼� ����
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void GenerateMission();
    //�̼� ����
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void MissionSuccess();
    //�̼� ����
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void MissionFail();

    //���� Ŭ���� ����
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void CheckWinCondition();
    //���� ���� ����
    UFUNCTION(BlueprintCallable, Category = "Game Flow")
    void CheckLoseCondition();


    //���̺� �ܰ� (����) ���
    UFUNCTION(BlueprintCallable, Category = "Player Stats")
    void PrintCurrentWave();
    //����
    UFUNCTION(BlueprintCallable, Category = "Player Stats")
    void PlayerHit(float Damage);
};
