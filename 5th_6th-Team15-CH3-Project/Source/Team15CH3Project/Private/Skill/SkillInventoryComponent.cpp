#include "Skill/SkillInventoryComponent.h"
#include "Skill/passiveItem.h"
#include "PlayerMade/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

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
    UWorld* World = GetWorld();
    APlayerCharacter* Player = nullptr;
    if (World)
    {
        Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(World, 0));
    }
    if (!Player || !World) return;


    if (HasPassiveSkill(NewSkill.Type))
    {
        for (auto& Skill : PassiveSkillsInv)
        {
            if (Skill.Type == NewSkill.Type)
            {
                Skill.StackCnt++;

                if (Skill.PassiveItemClass)
                {
                    FActorSpawnParameters SpawnParams;
                    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

                    ApassiveItem* TempItem = World->SpawnActor<ApassiveItem>(Skill.PassiveItemClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
                    if (TempItem)
                    {
                        TempItem->PassiveSkillData = Skill; // 전달할 데이터(스택 포함)
                        TempItem->PassiveType = Skill.Type;
                        TempItem->PassiveSkillApply(Player);

                        TempItem->Destroy();
                    }
                }

                return;
            }
        }
    }
    else
    {
        PassiveSkillsInv.Add(NewSkill);

        FPassiveItemData& AddedSkill = PassiveSkillsInv.Last();

        if (AddedSkill.PassiveItemClass)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

            ApassiveItem* TempItem = World->SpawnActor<ApassiveItem>(AddedSkill.PassiveItemClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
            if (TempItem)
            {
                TempItem->PassiveSkillData = AddedSkill; // 스택 기본값(예: 1) 포함
                TempItem->PassiveType = AddedSkill.Type;
                TempItem->PassiveSkillApply(Player);
                TempItem->Destroy();
            }
        }
    }
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
