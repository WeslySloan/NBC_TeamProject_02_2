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
	Movement->bOrientRotationToMovement = true; //Ű�� ĳ���Ͱ� �����̴� �������� �ٶ󺸰� ���� �̲��� ������ �ٶ󺻴�
	Movement->RotationRate = FRotator(0.0f, 540.0f, 0.0f); //���� ���� ȸ�� �ӵ� ������ ���ڱ� Ȯ ���ƺ���

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
