#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CommandCenter_GameManager.generated.h"

UCLASS()
class TEAM15CH3PROJECT_API ACommandCenter_GameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ACommandCenter_GameManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
