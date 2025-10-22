#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Widget_ActivatableBase.generated.h"

class APlayerMadeController;

UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class TEAM15CH3PROJECT_API UWidget_ActivatableBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintPure)
	APlayerMadeController* GetOwningAPlayerMadeController();

private:
	TWeakObjectPtr<APlayerMadeController> CachedOwningFrontendPC;
	
	
};
