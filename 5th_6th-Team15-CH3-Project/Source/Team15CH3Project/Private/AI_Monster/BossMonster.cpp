#include "AI_Monster/BossMonster.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "PlayerMade/PlayerCharacter.h"
#include "PlayerMade/CharacterStatsComponent.h"


ABossMonster::ABossMonster()
{
	PrimaryActorTick.bCanEverTick = false;

	// === 값은 CPP에서 설정 ===
	AttackDamage = 35.f;
	AttackRange = 220.f;
	MaxHP = 12000.f;
	CurrentHP = MaxHP;
	GoldReward = 500;

	AttackCooldown = 1.6f;
	LastAttackTime = -1000.f;

	// 이동 세팅
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->bOrientRotationToMovement = true;
		Movement->RotationRate = FRotator(0.f, 540.f, 0.f);
	}
}

void ABossMonster::BeginPlay()
{
	Super::BeginPlay();
	SetMovementSpeed(WalkSpeed);

	/*if (ABossMonster* MeleeCtrl = Cast<ABossMonster>(GetController()))
	{
		MeleeCtrl->StartChaseLoop();
		return;
	}
	if (ABossMonster* RangedCtrl = Cast<ABossMonster>(GetController()))
	{
		RangedCtrl->StartChaseLoop();
		return;
	}*/
}

void ABossMonster::SetMovementSpeed(float NewSpeed)
{
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->MaxWalkSpeed = NewSpeed;
	}
}

float ABossMonster::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	if (CurrentHP <= 0.f) return 0.f;

	const float Applied = FMath::Max(0.f, DamageAmount);
	CurrentHP = FMath::Clamp(CurrentHP - Applied, 0.f, MaxHP);

	UE_LOG(LogTemp, Warning, TEXT("[Boss] -%.1f HP  ->  %.1f / %.1f"),
		Applied, CurrentHP, MaxHP);

	if (CurrentHP <= 0.f)
	{
		//HandleDeath();
	}
	return Applied;
}
