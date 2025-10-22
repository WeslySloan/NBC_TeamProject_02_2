#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "Widget_PrimaryLayout.generated.h"

class UCommonActivatableWidgetContainerBase;

UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class TEAM15CH3PROJECT_API UWidget_PrimaryLayout : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UCommonActivatableWidgetContainerBase* FindRegisterStackByTag(const FGameplayTag& InTag) const;

protected:
	UFUNCTION(BlueprintCallable)
	void RegisteredWidgetStack(UPARAM(meta = (Categories = "Frontend.WidgetStack")) FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* InStack);

private:
	UPROPERTY(Transient)
	TMap <FGameplayTag, UCommonActivatableWidgetContainerBase*> RegisteredWidgetStackMap;

};
