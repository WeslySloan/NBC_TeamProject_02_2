#pragma once

#include "CoreMinimal.h"
#include "Skill/Base_item.h"
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
	EActiveSkillItemType Type = EActiveSkillItemType::None; //���ڸ��� ��ų�� Ÿ���� ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActiveSkillItem")
	FText SkillName;  //��ų �̸�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActiveSkillItem")
	FText Description; //��ųȿ���� ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActiveSkillItem")
	UTexture2D* Icon = nullptr; //�翬�� ��ų ������ �̹���

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	int32 StackCnt;
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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	EActiveSkillItemType ActiveType;
};
