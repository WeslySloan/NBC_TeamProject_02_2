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

	switch (PassiveType) // ���� ��� ���� �ٽ� ���� �ʿ�
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
	case EPassiveItemType::HPBoost: //�ִ� HP ���� ȿ��
		Stats->MaxHP *= (1.1f * (PassiveSkillData.StackCnt + 1));

		break;
	case EPassiveItemType::MPBoost: //�ִ� MP ���� ȿ��
		Stats->MaxMP *= (1.1f * (PassiveSkillData.StackCnt + 1));
		
		break;
	case EPassiveItemType::ExpBoost: //����ġ ȹ�淮 ���� ��AI ���� �޾ƿð� �ʿ�
		Stats->Experience *= (1.1f * (PassiveSkillData.StackCnt + 1));

		break;
	case EPassiveItemType::SprintBoost:
		Stats->MoveSpeed *= (1.1f * (PassiveSkillData.StackCnt + 1));

 		break;
	case EPassiveItemType::BloodAbsorbing: //���� ü���� ���� ȿ���� ȸ���ϴ� �нú�(��AI ���� �޾ƿð� �ʿ�)
		Stats->CurrentHP *= (1.1f * (PassiveSkillData.StackCnt + 1));

		break;
	case EPassiveItemType::AutomaticAttackDrone:

		break;
	default:
		break;
	}
}