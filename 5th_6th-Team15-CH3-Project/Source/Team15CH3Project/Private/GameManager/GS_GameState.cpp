#include "GameManager/GS_GameState.h"
#include "Net/UnrealNetwork.h"

AGS_GameState::AGS_GameState()
{
    Stamina = 100.0f;
    CurrentWave = 0;
    bBossDefeated = false;
    bMissionFailed = false;
    EnemyKillCount = 0;
}

void AGS_GameState::ModifyPlayerHealth(float Amount)
{
    Stamina = FMath::Clamp(Stamina + Amount, 0.0f, 100.0f);
}

void AGS_GameState::AddEnemyKillCount()
{
    EnemyKillCount++;
}

void AGS_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{

}