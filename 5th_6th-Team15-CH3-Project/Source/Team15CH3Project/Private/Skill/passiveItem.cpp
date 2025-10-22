#include "Skill/passiveItem.h"
#include "PlayerMade/PlayerCharacter.h"
ApassiveItem::ApassiveItem()
{
	StackCnt = 0;
	PassiveType = EPassiveItemType::None;
}

void ApassiveItem::BeginPlay()
{
	Super::BeginPlay();

	APlayerCharacter* Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Player)
	{
		PassiveSkillApply(Player);
	}
}

void ApassiveItem::PassiveSkillApply(APlayerCharacter* Target)
{
	switch (PassiveType)
	{
	case EPassiveItemType::AttackPowerBoost:

		break;
	case EPassiveItemType::AttackSpeedBoost:

		break;
	case EPassiveItemType::AttackIncreasingDirection:

		break;
	case EPassiveItemType::BulletBoost:

		break;
	case EPassiveItemType::MPBoost:

		break;
	case EPassiveItemType::ExpBoost:

		break;
	case EPassiveItemType::SprintBoost:

		break;
	case EPassiveItemType::BloodAbsorbing:

		break;
	case EPassiveItemType::AutomaticAttackDrone:

		break;
	default:
		break;
	}
}