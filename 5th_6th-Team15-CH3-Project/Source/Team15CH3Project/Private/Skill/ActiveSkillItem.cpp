#include "Skill/ActiveSkillItem.h"
#include "PlayerMade/PlayerCharacter.h"
#include "Skill/SkillUseIndicatorComponent.h"
#include "skill/Actor/PowerSurgeBullets.h"
#include "Skill/Actor/ShootingLasers.h"
#include "Skill/Actor/NuclearAttacks.h"
#include "Skill/Actor/NapalmAttacks.h"

AActiveSkillItem::AActiveSkillItem()
{
	ActiveSkillData.StackCnt = 0;
	ActiveSkillData.MaxStackCnt = 5;
	ActiveSkillData.PercentStack = 10.f;
	ActiveType = EActiveSkillItemType::None;
}

void AActiveSkillItem::BeginPlay()
{
	Super::BeginPlay();

	APlayerCharacter* Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

void AActiveSkillItem::ActiveSkillApply(class APlayerCharacter* Target)
{

	if (!Target) return;

	FActorSpawnParameters Params;
	Params.Owner = Target;
	Params.Instigator = Target;

	if (!ActiveSkillData.ActiveItemClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ActiveSkillApply: ActiveItemClass is NULL"));
		return;
	}

	if (ActiveSkillData.StackCnt < ActiveSkillData.MaxStackCnt)
	{
		ActiveSkillData.StackCnt++;
	}

	const float Multiplier = 1.0f + (ActiveSkillData.PercentStack / 100.0f) * ActiveSkillData.StackCnt;

	switch (ActiveType)
	{
	case EActiveSkillItemType::ShootingGun:
		if (APowerSurgeBullets* BulletCDO = Cast<APowerSurgeBullets>(ActiveSkillData.ActiveItemClass->GetDefaultObject()))
		{
			BulletCDO->Damage *= Multiplier;
		}
		break;

	case EActiveSkillItemType::CircleNearbySword:
		break;

	case EActiveSkillItemType::ShootingLaser:
		if (AShootingLasers* ShotLaser = Cast<AShootingLasers>(ActiveSkillData.ActiveItemClass->GetDefaultObject()))
		{
			ShotLaser->Damage *= Multiplier;
		}
		break;
	case EActiveSkillItemType::NapalmAttack:
		if (ANapalmAttacks* NapalmAtk = Cast<ANapalmAttacks>(ActiveSkillData.ActiveItemClass->GetDefaultObject()))
		{
			NapalmAtk->Damage *= Multiplier;
		}
		break;
	case EActiveSkillItemType::NuclearAttack:
		if (ANuclearAttacks* NuclearAtk = Cast<ANuclearAttacks>(ActiveSkillData.ActiveItemClass->GetDefaultObject()))
		{
			NuclearAtk->Damage *= Multiplier;
		}
		break;
	default:
		break;
	}
}
