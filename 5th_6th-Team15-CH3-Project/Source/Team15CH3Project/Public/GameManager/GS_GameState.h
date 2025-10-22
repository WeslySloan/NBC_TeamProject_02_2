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

    //�÷��̾� ü��
    UPROPERTY(BlueprintReadWrite, Replicated, Category = "Player Stats")
    float Stamina;

    //���� ���̺�
    UPROPERTY(BlueprintReadWrite, Replicated, Category = "Wave Stats")
    int32 CurrentWave;

    //���� óġ ����
    UPROPERTY(BlueprintReadWrite, Replicated, Category = "Wave Stats")
    bool bBossDefeated;

    //�̼� ���� ����
    UPROPERTY(BlueprintReadWrite, Replicated, Category = "Mission")
    bool bMissionFailed;

    //�� óġ ��
    UPROPERTY(BlueprintReadWrite, Replicated, Category = "Player Stats")
    int32 EnemyKillCount;

private:
    // �÷��̾� ü�� ����
    void ModifyPlayerHealth(float Amount);

    // �� óġ ����
    void AddEnemyKillCount();
};
