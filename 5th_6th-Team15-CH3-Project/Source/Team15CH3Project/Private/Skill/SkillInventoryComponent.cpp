#include "Skill/SkillInventoryComponent.h"

USkillInventoryComponent::USkillInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USkillInventoryComponent::AddActiveSkill(const FActiveSkillItemData& NewSkill)
{
	if (HasActiveSkill(NewSkill.Type))
	{
		for (auto& Skill : ActiveSkillsInv) // �̹� ���� ���̸� ���� ����(AI�ƴ� ��¥ ���������� ^^)
		{
			if (Skill.Type == NewSkill.Type)
			{
				Skill.StackCnt++; 
				return;
			}
		}
	}
	ActiveSkillsInv.Add(NewSkill); //ó�� ȹ���ϴ� ��ų�̸� ��ų �κ��� ���� �߰�
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
