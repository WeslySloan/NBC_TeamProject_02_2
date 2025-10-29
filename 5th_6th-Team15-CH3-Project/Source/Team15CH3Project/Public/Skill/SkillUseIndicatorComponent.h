// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/WidgetComponent.h"
#include "SkillUseIndicatorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM15CH3PROJECT_API USkillUseIndicatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkillUseIndicatorComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ShowIndicator();
	void HideIndicator();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Indicator")
	TSubclassOf<AActor> IndicatorActorClass;

	UPROPERTY()
	AActor* SpawnedIndicatorActor;

private:
	bool bIsVisible;

	void UpdateIndicatorLocation();
		
};
