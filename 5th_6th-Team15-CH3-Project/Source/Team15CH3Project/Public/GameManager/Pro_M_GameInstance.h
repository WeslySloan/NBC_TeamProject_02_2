#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Skill/SkillDataAsset.h"
#include "Pro_M_GameInstance.generated.h"

UCLASS()
class TEAM15CH3PROJECT_API UPro_M_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	USkillDataAsset* SkillDataAsset;
};
