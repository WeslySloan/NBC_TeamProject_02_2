// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

class TEAM15CH3PROJECT_API IItemInterface
{
	GENERATED_BODY()

public:
	virtual void ActivateItem(AActor* Acivator) = 0;
	virtual FName GetIemType() const = 0; //FString 보단 타입 분별은 FName이 더 빠름
};
