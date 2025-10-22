// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager/GM_GameMode.h"
#include "GameManager/GS_GameState.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AGM_GameMode::AGM_GameMode()
{
    // Tick Ȱ��ȭ 
    PrimaryActorTick.bCanEverTick = true;

    // �⺻�� 
    bIsGameOver = false;
    MissionSpawnDelay = 10.0f;

    // GameState Ŭ���� ���� 
    GameStateClass = AGS_GameState::StaticClass();
}

void AGM_GameMode::BeginPlay()
{
    Super::BeginPlay();
    StartGame();
}

void AGM_GameMode::StartGame()
{
    bIsGameOver = false;

    // ���ӽ�����Ʈ �ҷ�����
    if (AGS_GameState* GS = GetGameState<AGS_GameState>())
    {
        // ó�� ������ �ʱ�ȭ
        GS->EnemyKillCount= 0;
        GS->Stamina = 100.0f;
        GS->CurrentWave = 0;
        GS->bBossDefeated = false;
        GS->bMissionFailed = false;
    }

    ResetGame();

    // ���� �ð� �� �̼� ���� 
    GetWorldTimerManager().SetTimer(
        MissionTimerHandle,
        this,
        &AGM_GameMode::GenerateMission,
        MissionSpawnDelay,
        false
    );
    //ù��° ���̺�
    StartNextWave();
}

void AGM_GameMode::ResetGame()
{
    //�÷��̾� ��Ʈ�� ���� �� ���� �ۼ��ϱ�
}


void AGM_GameMode::StartNextWave()
{
    if (bIsGameOver) return;

    if (AGS_GameState* GS = GetGameState<AGS_GameState>())
    {
        // ���� ���̺�
        GS->CurrentWave++;
    }
    // �� ����
    SpawnEnemiesForWave();
}

void AGM_GameMode::SpawnEnemiesForWave()
{
    // 1.�� ���� ���� �ۼ��ϱ�
}

void AGM_GameMode::GenerateMission()
{
    // 2.���� �̼� ���� ���� �ۼ��ϱ�
}

void AGM_GameMode::MissionSuccess()
{
    // �̼� ������ ĳ���� ��ȭ ���� �ۼ����ּ���
}

void AGM_GameMode::MissionFail()
{
    // �̼� ���н� �� ��ȭ ���� �ۼ����ּ���
}

void AGM_GameMode::CheckWinCondition()
{
    if (AGS_GameState* GS = GetGameState<AGS_GameState>())
    {
        // ������ óġ �� ���� ����
        if (GS->bBossDefeated)
        {
            EndGame();
        }
    }
}

void AGM_GameMode::CheckLoseCondition()
{
    if (AGS_GameState* GS = GetGameState<AGS_GameState>())
    {
        // ü�� 0 ������ �� ���� ����
        if (GS->Stamina <= 0)
        {
            EndGame();
        }
    }
}

void AGM_GameMode::EndGame()
{
    bIsGameOver = true;

    if (AGS_GameState* GS = GetGameState<AGS_GameState>())
    {
        //���� ���� UI ���
    }
}

void AGM_GameMode::PrintCurrentWave()
{
    if (bIsGameOver) return;

    if (AGS_GameState* GS = GetGameState<AGS_GameState>())
    {
        //UI������� �ٲٱ�
        UE_LOG(LogTemp, Log, TEXT("���� ���̺�: %d"), GS->CurrentWave);
    }
}


void AGM_GameMode::PlayerHit(float Damage)
{
    if (bIsGameOver) return;

    if (AGS_GameState* GS = GetGameState<AGS_GameState>())
    {
        //���� ó�� �� �й� ���� Ȯ��
        //GS->ModifyPlayerHealth(-Damage);
        CheckLoseCondition();
    }
}
