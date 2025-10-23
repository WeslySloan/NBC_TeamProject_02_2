#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "passiveItem.h"
#include "ActiveSkillItem.h"
#include "SkillDataAsset.generated.h"

UCLASS(BlueprintType)
class TEAM15CH3PROJECT_API USkillDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
    // 패시브 스킬 데이터들
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    TArray<FPassiveItemData> PassiveSkills;

    // 액티브 스킬 데이터들
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    TArray<FActiveSkillItemData> ActiveSkills;
};
