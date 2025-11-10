#pragma once

#include "CoreMinimal.h"
#include "Skill/Base_item.h"
#include "Skill/Actor/Drone.h"
#include "passiveItem.generated.h"

UENUM(BlueprintType)
enum class EPassiveItemType : uint8
{
	None UMETA(DisplayName = "None"),
	AttackPowerBoost UMETA(DisplayName = "Attack Power Boost"),
	AttackSpeedBoost UMETA(DisplayName = "Attack Speed Boost"),
	AttackIncreasingDirection UMETA(DisplayName = "Attack Increasing Direction"),
	BulletBoost UMETA(DisplayName = "Bullet Boost"),
	HPBoost UMETA(DisplayName = "HP Boost"),
	MPBoost UMETA(DisplayName = "MP Boost"),
	ExpBoost UMETA(DisplayName = "Exp Boost"),
	BloodAbsorbing UMETA(DisplayName = "Blood Absording"),
	SprintBoost UMETA(DisplayName = "Spint Boost"),
	AutomaticAttackDrone UMETA(DisplayName = "Automatic Attack Drone")
};

USTRUCT(BlueprintType)
struct FPassiveItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PassiveSkill")
	EPassiveItemType Type = EPassiveItemType::None; //각자마다 스킬에 타입을 나눠 놨음

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PassiveSkill")
	FText SkillName;  //스킬 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PassiveSkill")
	FText Description; //스킬효과에 대한 설명

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PassiveSkill")
	UTexture2D* Icon = nullptr; //당연히 스킬 아이콘 이미지

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PassiveSkill")
	int32 StackCnt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PassiveSkill")
	int32 MaxStackCnt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PassiveSkill")
	int32 CoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PercentStack = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PassiveSkill")
	TSubclassOf<class ApassiveItem> PassiveItemClass;
};

UCLASS()
class TEAM15CH3PROJECT_API ApassiveItem : public ABase_item
{
	GENERATED_BODY()
public:
	ApassiveItem();

	virtual void BeginPlay() override;
	void PassiveSkillApply(class APlayerCharacter* Target);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FPassiveItemData PassiveSkillData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	EPassiveItemType PassiveType;

	UPROPERTY(EditAnywhere, Category = "Drone")
	TSubclassOf<ADrone> DroneClass;
};


