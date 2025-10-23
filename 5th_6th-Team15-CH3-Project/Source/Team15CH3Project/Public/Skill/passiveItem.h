#pragma once

#include "CoreMinimal.h"
#include "Skill/Base_item.h"
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
	EPassiveItemType Type = EPassiveItemType::None; //���ڸ��� ��ų�� Ÿ���� ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PassiveSkill")
	FText SkillName;  //��ų �̸�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PassiveSkill")
	FText Description; //��ųȿ���� ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PassiveSkill")
	UTexture2D* Icon = nullptr; //�翬�� ��ų ������ �̹���

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PassiveSkill")
	int32 StackCnt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PassiveSkill")
	int32 MaxStackCnt;

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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	EPassiveItemType PassiveType;
};


