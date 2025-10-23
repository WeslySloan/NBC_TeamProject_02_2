#include "Skill/SkillManager.h"

ASkillManager::ASkillManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASkillManager::BeginPlay()
{
    Super::BeginPlay();

    if (SkillDataAsset)
    {
        for (const FPassiveItemData& Passive : SkillDataAsset->PassiveSkills)// 패시브 스킬 데이터들 추가
        {
            FSkillPool NewSkill;
            NewSkill.bIsPassive = true;
            NewSkill.PassiveData = Passive;
            SkillPool.Add(NewSkill);
        }

        for (const FActiveSkillItemData& Active : SkillDataAsset->ActiveSkills)// 액티브 스킬 데이터들 추가
        {
            FSkillPool NewSkill;
            NewSkill.bIsPassive = false;
            NewSkill.ActiveData = Active;
            SkillPool.Add(NewSkill);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("SkillManager: SkillDataAsset is not assigned!"));
    }
}
