#include "Skill/passiveItem.h"
#include "PlayerMade/CharacterStatsComponent.h"
#include "PlayerMade/PlayerCharacter.h"
ApassiveItem::ApassiveItem()
{
	PassiveSkillData.StackCnt = 0;
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
	if (!Target) return;

	UCharacterStatsComponent* Stats = Target->FindComponentByClass<UCharacterStatsComponent>();
	if (!Stats) return;

	switch (PassiveType) // 스탯 계산 수식 다시 정립 필요
	{
	case EPassiveItemType::AttackPowerBoost:
		Stats->AttackDamage *= (5.0f * (PassiveSkillData.StackCnt + 1));

		break;
	case EPassiveItemType::AttackSpeedBoost:
		Stats->AttackSpeed *= (1.1f * (PassiveSkillData.StackCnt + 1));

		break;
	case EPassiveItemType::AttackIncreasingDirection:
		
		break;
	case EPassiveItemType::BulletBoost:

		break;
	case EPassiveItemType::HPBoost: //최대 HP 증가 효과
		Stats->MaxHP *= (1.1f * (PassiveSkillData.StackCnt + 1));

		break;
	case EPassiveItemType::MPBoost: //최대 MP 증가 효과
		Stats->MaxMP *= (1.1f * (PassiveSkillData.StackCnt + 1));
		
		break;
	case EPassiveItemType::ExpBoost: //경험치 획득량 증가 적AI 에서 받아올게 필요
		Stats->Experience *= (1.1f * (PassiveSkillData.StackCnt + 1));

		break;
	case EPassiveItemType::SprintBoost:
		Stats->MoveSpeed *= (1.1f * (PassiveSkillData.StackCnt + 1));

 		break;
	case EPassiveItemType::BloodAbsorbing: //현재 체력을 흡혈 효과로 회복하는 패시브(적AI 에서 받아올게 필요)
		Stats->CurrentHP *= (1.1f * (PassiveSkillData.StackCnt + 1));

		break;
	case EPassiveItemType::AutomaticAttackDrone:

		break;
	default:
		break;
	}
}