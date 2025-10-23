#include "Skill/ActiveSkillItem.h"
#include "PlayerMade/PlayerCharacter.h"
AActiveSkillItem::AActiveSkillItem()
{
	ActiveSkillData.StackCnt = 0;
	ActiveType = EActiveSkillItemType::None;
}

void AActiveSkillItem::BeginPlay()
{
	Super::BeginPlay();

	APlayerCharacter* Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Player)
	{
		ActiveSkillApply(Player);
	}
}

void AActiveSkillItem::ActiveSkillApply(class APlayerCharacter* Target)
{
	switch (ActiveType)
	{
	case EActiveSkillItemType::ShootingGun:

		break;
	case EActiveSkillItemType::CircleNearbySword:

		break;
	case EActiveSkillItemType::ShootingLaser:

		break;
	case EActiveSkillItemType::NapalmAttack:

		break;
	case EActiveSkillItemType::NuclearAttack:

		break;
	default:
		break;
	}
}
