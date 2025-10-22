#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "FrontendFunctionLibrary.generated.h"

class UWidget_ActivatableBase;

UCLASS()
class TEAM15CH3PROJECT_API UFrontendFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Frontend Function Library")
	static TSoftClassPtr<UWidget_ActivatableBase> GetFrontendSoftWidgetClassByTag(UPARAM(meta = (Categories = "Frontend.Widget")) FGameplayTag InWidgetTag);
	
};
