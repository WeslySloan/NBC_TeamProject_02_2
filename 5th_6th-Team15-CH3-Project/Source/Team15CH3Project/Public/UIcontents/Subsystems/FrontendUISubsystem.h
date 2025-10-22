#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FrontendUISubsystem.generated.h"


class UWidget_PrimaryLayout;
struct FGameplayTag;
class UWidget_ActivatableBase;

enum class EAsyncPushWidgetState : uint8
{
	OnCreatedBeforePush,
	AfterPush
};


UCLASS()
class TEAM15CH3PROJECT_API UFrontendUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()


public:
	static UFrontendUISubsystem* Get(const UObject* WorldContextObject);

	//~ Begin USubsyem Interface
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	//~ End USubsyem Interface

	UFUNCTION(BlueprintCallable)
	void RegisterCreatedPrimaryLayoutWidget(UWidget_PrimaryLayout* InCreatedWidget);

	void PushSoftWidgetToStackAynsc(const FGameplayTag& InWidgetStackTag, TSoftClassPtr<UWidget_ActivatableBase> 
		InSoftWidgetClass, TFunction<void(EAsyncPushWidgetState, UWidget_ActivatableBase*)> AysncPushStateCallback);
private:
	UPROPERTY(Transient)
	UWidget_PrimaryLayout* CreatedPrimaryLayout;
};
