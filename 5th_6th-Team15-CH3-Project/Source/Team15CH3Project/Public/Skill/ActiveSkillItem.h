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

UCLASS()
class TEAM15CH3PROJECT_API AActiveSkillItem : public ABase_item
{
	GENERATED_BODY()

public:
	AActiveSkillItem();
	
	virtual void BeginPlay() override;
	void ActiveSkillApply(class APlayerCharacter* Target);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	int32 StackCnt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	EActiveSkillItemType ActiveType;
};
