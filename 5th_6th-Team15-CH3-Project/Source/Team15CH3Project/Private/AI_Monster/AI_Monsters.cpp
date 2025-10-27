#include "AI_Monster/AI_Monsters.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI_Monster/AI_MonsterController.h"


AAI_Monsters::AAI_Monsters()
{

	AIControllerClass = AAI_Monsters::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->MaxWalkSpeed = WalkSpeed;
	Movement->bOrientRotationToMovement = true; //Ű�� ĳ���Ͱ� �����̴� �������� �ٶ󺸰� ���� �̲��� ������ �ٶ󺻴�
	Movement->RotationRate = FRotator(0.0f, 540.0f, 0.0f); //���� ���� ȸ�� �ӵ� ������ ���ڱ� Ȯ ���ƺ���

}


void AAI_Monsters::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("[Monster] AI Character has been spawned."));
}

void AAI_Monsters::SetMovemonetSpeed(float NewSpeed)
{
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->MaxWalkSpeed = NewSpeed;
	}
}

