#include "Skill/passiveItem.h"
#include "PlayerMade/CharacterStatsComponent.h"
#include "PlayerMade/PlayerCharacter.h"
ApassiveItem::ApassiveItem()
{
	PassiveSkillData.StackCnt = 0;
	PassiveSkillData.MaxStackCnt = 5;
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

	if (PassiveSkillData.StackCnt < PassiveSkillData.MaxStackCnt)
	{
		PassiveSkillData.StackCnt++;
	}

	const float BaseDamage = 10.0f;
	const float BaseAttackSpeed = 10.0f;
	const float BaseMaxHP = 10.0f;
	const float BaseMaxMP = 10.0f;
	const float BaseExp = 10.0f;
	const float BaseMoveSpeed = 10.0f;
	const float BaseCurrentHP = 10.0f;


	switch (PassiveType) // 스탯 계산 수식 다시 정립 필요
	{
	case EPassiveItemType::AttackPowerBoost:
		Stats->AttackDamage = BaseDamage * (1.0f + 0.1f * PassiveSkillData.StackCnt);

		break;
	case EPassiveItemType::AttackSpeedBoost:
		Stats->AttackSpeed = BaseAttackSpeed * (1.0f + 0.1f * PassiveSkillData.StackCnt);

		break;
	case EPassiveItemType::AttackIncreasingDirection:
		
		break;
	case EPassiveItemType::BulletBoost:

		break;
	case EPassiveItemType::HPBoost: //최대 HP 증가 효과
		Stats->MaxHP = BaseMaxHP * (1.0f + 0.1f * PassiveSkillData.StackCnt);

		break;
	case EPassiveItemType::MPBoost: //최대 MP 증가 효과
		Stats->MaxMP = BaseMaxMP * (1.0f + 0.1f * PassiveSkillData.StackCnt);
		
		break;
	case EPassiveItemType::ExpBoost: //경험치 획득량 증가 적AI 에서 받아올게 필요
		

		break;
	case EPassiveItemType::SprintBoost:
		Stats->MoveSpeed = BaseMoveSpeed * (1.0f + 0.1f * PassiveSkillData.StackCnt);

 		break;
	case EPassiveItemType::BloodAbsorbing: //현재 체력을 흡혈 효과로 회복하는 패시브(적AI 에서 받아올게 필요)
		Stats->CurrentHP;

		break;
	case EPassiveItemType::AutomaticAttackDrone:

		break;
	default:
		break;
	}

	UE_LOG(LogTemp, Warning, TEXT("[PassiveSkill] %s 획득 스택: %d"),
		*PassiveSkillData.SkillName.ToString(), PassiveSkillData.StackCnt);

	UE_LOG(LogTemp, Warning, TEXT("향상된 능력치: AttackDamage=%.1f, AttackSpeed=%.1f, MaxHP=%.1f, MaxMP=%.1f, MoveSpeed=%.1f"),
		Stats->AttackDamage, Stats->AttackSpeed, Stats->MaxHP, Stats->MaxMP, Stats->MoveSpeed);
}