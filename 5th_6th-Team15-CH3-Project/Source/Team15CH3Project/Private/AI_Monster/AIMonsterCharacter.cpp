#include "AI_Monster/AIMonsterCharacter.h"
#include "AI_Monster/MonsterAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AAIMonsterCharacter::AAIMonsterCharacter()
{
	AIControllerClass = AMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;



	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->MaxWalkSpeed = WalkSpeed;
	Movement->bOrientRotationToMovement = true; //키면 캐릭터가 움직이는 방향으로 바라보고 끄면 미끌어 지듯이 바라본다
	Movement->RotationRate = FRotator(0.0f, 540.0f, 0.0f); //돌아 보는 회전 속도 높으면 갑자기 확 돌아본다

}

void AAIMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("[Monster] AI Character has been spawned."));
}

void AAIMonsterCharacter::SetMovemonetSpeed(float NewSpeed)
{
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->MaxWalkSpeed = NewSpeed;
	}
}
