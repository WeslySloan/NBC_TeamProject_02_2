#pragma once

#include "CoreMinimal.h"
#include "Skill/Base_item.h"
#include "TimerManager.h"
#include "Skill/Actor/PowerSurgeBullet.h"
#include "ActiveSkillItem.generated.h"

UENUM(BluePrintType)
enum class EActiveSkillItemType : uint8
{
	None UMETA(DisplayName = "None"),
	ShootingGun UMETA(DisplayName = "Shooting Gun"),
	CircleNearbySword UMETA(DisplayName = "Circle Neraby Sword"),
	ShootingLaser UMETA(DisplayName = "Shooting Laser"),
	NapalmAttack UMETA(DisplayName = "Napalm Attack"),
	NuclearAttack UMETA(DisplayName = "Nuclear Attck"),
};

USTRUCT(BlueprintType)
struct FActiveSkillItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActiveSkillItem")
	EActiveSkillItemType Type = EActiveSkillItemType::None; //각자마다 스킬에 타입을 나눠 놨음

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActiveSkillItem")
	FText SkillName;  //스킬 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActiveSkillItem")
	FText Description; //스킬효과에 대한 설명

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActiveSkillItem")
	UTexture2D* Icon = nullptr; //당연히 스킬 아이콘 이미지

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	int32 StackCnt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActiveSkillItem")
	int32 MaxStackCnt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActiveSkillItem")
	int32 CoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActiveSkillItem")
	int32 MaxCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActiveSkillItem")
	bool bIsCoolingDown = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActiveSkillItem")
	bool UseingSkill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActiveSkillItem")
	int32 UseMana;

	FTimerHandle CooldownTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PercentStack = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkilI")
	TSubclassOf<AActor> ActiveItemClass;

};

UCLASS()
class TEAM15CH3PROJECT_API AActiveSkillItem : public ABase_item
{
	GENERATED_BODY()

public:
	AActiveSkillItem();
	
	virtual void BeginPlay() override;
	void ActiveSkillApply(class APlayerCharacter* Target);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FActiveSkillItemData ActiveSkillData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	EActiveSkillItemType ActiveType;

private:
	FTimerHandle CooldownTimerHandle;
};
