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
	MPBoost UMETA(DisplayName = "MP Boost"),
	ExpBoost UMETA(DisplayName = "Exp Boost"),
	BloodAbsorbing UMETA(DisplayName = "Blood Absording"),
	SprintBoost UMETA(DisplayName = "Spint Boost"),
	AutomaticAttackDrone UMETA(DisplayName = "Automatic Attack Drone")
};

UCLASS()
class TEAM15CH3PROJECT_API ApassiveItem : public ABase_item
{
	GENERATED_BODY()
public:
	ApassiveItem();

	virtual void BeginPlay() override;
	void PassiveSkillApply(class APlayerCharacter* Target);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	int32 StackCnt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	EPassiveItemType PassiveType;
};
