#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerMadeController.generated.h"



UCLASS()
class TEAM15CH3PROJECT_API APlayerMadeController : public APlayerController
{
	GENERATED_BODY()
	// ~Begin Controller Interface
	virtual void OnPossess(APawn* aPawn) override;
	// ~End Controller Interface
};
