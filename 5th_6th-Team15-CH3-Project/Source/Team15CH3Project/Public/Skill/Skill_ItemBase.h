// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Skill_ItemBase.generated.h"

class PlayerCharactor;
 
UCLASS()
class TEAM15CH3PROJECT_API USkill_ItemBase : public UObject
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FName SkillName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float CoolTime;

    virtual void Activate(PlayerCharactor* Owner) PURE_VIRTUAL(USkillBase::Activate, );
};
