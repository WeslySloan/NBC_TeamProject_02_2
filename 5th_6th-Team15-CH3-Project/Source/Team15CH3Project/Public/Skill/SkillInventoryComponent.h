// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "passiveItem.h"
#include "ActiveSkillItem.h"
#include "SkillInventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM15CH3PROJECT_API USkillInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkillInventoryComponent();

	//��Ƽ�� ��ų
	UFUNCTION(BlueprintCallable, Category = "SkillInventory | Active")
	void AddActiveSkill(const FActiveSkillItemData& NewSkill); // ��ų �߰�

	UFUNCTION(BlueprintCallable, Category = "SkillInventory | Active")
	bool HasActiveSkill(EActiveSkillItemType Type) const; //��ų �ߺ� üũ �ߺ��� �ִٸ� ��ø ���ٸ� ��ų �߰� ����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Inventory | Active")
	TArray<FActiveSkillItemData> ActiveSkillsInv; //��ų �κ��丮 ����

	//�нú� ��ų
	UFUNCTION(BlueprintCallable, Category = "SkillInventory | Passive")
	void AddPassiveSkill(const FPassiveItemData& NewSkill); 

	UFUNCTION(BlueprintCallable, Category = "SkillInventory | Passive")
	bool HasPassiveSkill(EPassiveItemType Type) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Inventory | Passive")
	TArray<FPassiveItemData> PassiveSkillsInv;

	UFUNCTION(BlueprintCallable, Category = "Skill Inventory | Utility")
	void ClearAllSkills(); // ��ųâ ����
};
