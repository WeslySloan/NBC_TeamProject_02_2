#include "AI_Monster/AI_Monsters.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI_Monster/AI_MonsterController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"


AAI_Monsters::AAI_Monsters()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AAI_MonsterController::StaticClass();
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->bOrientRotationToMovement = true; //키면 캐릭터가 움직이는 방향으로 바라보고 끄면 미끌어 지듯이 바라본다
	Movement->RotationRate = FRotator(0.0f, 540.0f, 0.0f); //돌아 보는 회전 속도 높으면 갑자기 확 돌아본다
	Movement->MaxWalkSpeed = WalkSpeed;
	CurrentHP = MaxHP;

	AttackRange = 150.f;
	AttackCooldown = 1.5f;
	AttackDamage = 15.f;
}


void AAI_Monsters::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("[Monster] AI Character has been spawned."));

	SetMovementSpeed(WalkSpeed);
	AAI_MonsterController* MonsterController = Cast< AAI_MonsterController>(GetController());
	MonsterController->StartChaseLoop();
}

void AAI_Monsters::SetMovementSpeed(float NewSpeed)
{
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->MaxWalkSpeed = NewSpeed;
	}
}

bool AAI_Monsters::CanAttack(APawn* Target) const
{
	if (!Target || IsDead()) return false;
	const float Now = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;
	if (Now - LastAttackTime < AttackCooldown) return false;
	const float Dist = FVector::Dist2D(GetActorLocation(), Target->GetActorLocation());
	return Dist <= AttackRange;
}

void AAI_Monsters::PerformAttack(APawn* Target)
{
	if (!Target || IsDead()) return;

	LastAttackTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;

	UE_LOG(LogTemp, Warning, TEXT("[Monster] Attack! Target=%s, Damage=%.1f, Dist=%.0f"), *Target->GetName(), AttackDamage, FVector::Dist2D(GetActorLocation(), Target->GetActorLocation()));


}

