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
    // �нú� ��ų �����͵�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    TArray<FPassiveItemData> PassiveSkills;

    // ��Ƽ�� ��ų �����͵�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    TArray<FActiveSkillItemData> ActiveSkills;
};
