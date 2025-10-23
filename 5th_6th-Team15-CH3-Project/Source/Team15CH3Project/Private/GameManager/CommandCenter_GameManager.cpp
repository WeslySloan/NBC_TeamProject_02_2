#include "GameManager/CommandCenter_GameManager.h"

ACommandCenter_GameManager::ACommandCenter_GameManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACommandCenter_GameManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACommandCenter_GameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

