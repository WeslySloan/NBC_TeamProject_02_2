#include "Skill/SkillInventoryComponent.h"

USkillInventoryComponent::USkillInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USkillInventoryComponent::AddActiveSkill(const FActiveSkillItemData& NewSkill)
{
	if (HasActiveSkill(NewSkill.Type))
	{
		for (auto& Skill : ActiveSkillsInv) // 이미 보유 중이면 스택 증가(AI아님 진짜 내가쓴거임 ^^)
		{
			if (Skill.Type == NewSkill.Type)
			{
				Skill.StackCnt++; 
				return;
			}
		}
	}
	ActiveSkillsInv.Add(NewSkill); //처음 획득하는 스킬이면 스킬 인벤에 직접 추가
}

bool USkillInventoryComponent::HasActiveSkill(EActiveSkillItemType Type) const
{
	for (const auto& Skill : ActiveSkillsInv)
	{
		if (Skill.Type == Type)
			return true;
	}
	return false;
}

void USkillInventoryComponent::AddPassiveSkill(const FPassiveItemData& NewSkill)
{
	if (HasPassiveSkill(NewSkill.Type))
	{
		for (auto& Skill : PassiveSkillsInv)
		{
			if (Skill.Type == NewSkill.Type)
			{
				Skill.StackCnt++;
				return;
			}
		}
	}
	PassiveSkillsInv.Add(NewSkill);
}

bool USkillInventoryComponent::HasPassiveSkill(EPassiveItemType Type) const
{
	for (const auto& Skill : PassiveSkillsInv)
	{
		if (Skill.Type == Type)
			return true;
	}
	return false;
}

void USkillInventoryComponent::ClearAllSkills()
{
	ActiveSkillsInv.Empty();
	PassiveSkillsInv.Empty();
}
