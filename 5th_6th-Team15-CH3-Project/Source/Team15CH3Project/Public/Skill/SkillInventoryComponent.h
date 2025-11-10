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

	//액티브 스킬
	UFUNCTION(BlueprintCallable, Category = "SkillInventory | Active")
	void AddActiveSkill(const FActiveSkillItemData& NewSkill); // 스킬 추가

	UFUNCTION(BlueprintCallable, Category = "SkillInventory | Active")
	bool HasActiveSkill(EActiveSkillItemType Type) const; //스킬 중복 체크 중복이 있다면 중첩 없다면 스킬 추가 형식

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkillInventory | Active")
	TArray<FActiveSkillItemData> ActiveSkillsInv; //스킬 인벤토리 저장

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillInventory | Active")
	TSubclassOf<AActiveSkillItem> ActiveItemClass;

	//패시브 스킬
	UFUNCTION(BlueprintCallable, Category = "SkillInventory | Passive")
	void AddPassiveSkill(const FPassiveItemData& NewSkill); 

	UFUNCTION(BlueprintCallable, Category = "SkillInventory | Passive")
	bool HasPassiveSkill(EPassiveItemType Type) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Inventory | Passive")
	TArray<FPassiveItemData> PassiveSkillsInv;

	UFUNCTION(BlueprintCallable, Category = "Skill Inventory | Utility")
	void ClearAllSkills(); // 스킬창 비우기

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Passive Skill")
	TSubclassOf<class ApassiveItem> PassiveItemClass;
};
