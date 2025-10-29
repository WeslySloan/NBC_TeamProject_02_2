// PlayerCharacter.cpp

#include "PlayerMade/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "PlayerMade/CharacterStatsComponent.h"
#include "Components/CapsuleComponent.h"
#include "PlayerMade/AutoAttackComponent.h" // 💡 [추가] AutoAttackComponent 사용을 위해 필요

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// === 1. 탑다운 카메라 설정 ===
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	CameraBoom->TargetArmLength = 1500.0f;
	CameraBoom->SetRelativeRotation(FRotator(-120.0f, 0.0f, 0.0f));
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	StatsComponent = CreateDefaultSubobject<UCharacterStatsComponent>(TEXT("StatsComponent"));

	// === 2. 이동 및 회전 설정 ===
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);

	// 💡 [추가] 이동 즉각성 향상을 위해 가속/감속 기본값 설정
	// MaxWalkSpeed는 BeginPlay에서 StatsComponent 값으로 덮어쓰입니다.
	GetCharacterMovement()->MaxAcceleration = 99999.0f; // 가속도를 최대로 올려 즉각 이동
	GetCharacterMovement()->BrakingDecelerationWalking = 99999.0f; // 감속도를 최대로 올려 즉각 정지
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 💡 [수정] BeginPlay에서 MaxWalkSpeed 설정 로직을 분리하여 Move 함수 로직 제거 및 정리
	if (UCharacterStatsComponent* StatsComp = FindComponentByClass<UCharacterStatsComponent>())
	{
		GetCharacterMovement()->MaxWalkSpeed = StatsComp->MoveSpeed;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		// 마우스 커서 표시
		PlayerController->bShowMouseCursor = true;
		PlayerController->DefaultMouseCursor = EMouseCursor::Crosshairs;

		// Enhanced Input Context 적용
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enhanced Input 바인딩
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		}
	}
}

// Move 콜백 함수 (WASD 처리)
void APlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// 카메라의 Yaw 회전을 기준으로 방향을 계산 (탑다운 시점 이동)
		// const FRotator Rotation = CameraBoom->GetComponentRotation();
		//const FRotator Rotation = Controller->GetControlRotation();
		//const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		if (!MovementVector.IsNearlyZero())
		{
			const FVector MoveDir = (ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X).GetSafeNormal();
			SetActorRotation(MoveDir.Rotation());
		}

		// if (UCharacterStatsComponent* StatsComp = FindComponentByClass<UCharacterStatsComponent>())
		// {
		// 	GetCharacterMovement()->MaxWalkSpeed = StatsComp->MoveSpeed; //// maxwalkspeed 상태여서 속도에 가속이 적용된상태 -> 속도 일정하도록 
		// }


		//if (!FMath::IsNearlyZero(MovementVector.Y))
		//{
		//	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		//	AddMovementInput(ForwardDirection, MovementVector.Y);
		//	/*FRotator(0.0f, 0.0f, 0.0f);*/
		//}

		//if (!FMath::IsNearlyZero(MovementVector.X))
		//{
		//	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		//	AddMovementInput(RightDirection, MovementVector.X);
		//}
	}
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!StatsComponent)
	{
		return 0.0f;
	}

	// StatsComponent의 데미지 처리 로직 호출
	const float ActualDamage = StatsComponent->TakeDamage(DamageAmount);

	// 사망했는지 확인하고, 캐릭터의 물리적 반응 처리
	if (StatsComponent->IsDead())
	{
		PlayerIsDead();
	}

	return ActualDamage;
}

void APlayerCharacter::PlayerIsDead()
{
	UE_LOG(LogTemp, Error, TEXT("PLAYER IS DEAD! Deactivating input and collision."));

	// 1. 입력 비활성화
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		DisableInput(PC);
	}

	// 2. 캐릭터 충돌 비활성화
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// AutoAttackComponent를 찾아서 타이머를 정지합니다.
	//if (AutoAttackComponent)
	//{
	//	AutoAttackComponent->StopAutoAttack(); // AutoAttackComponent에 StopAutoAttack() 함수가 있다고 가정합니다.
	//}

	// 5초 후에 액터를 제거하는 로직을 추가합니다. (바로 제거하면 로그가 잘 안 보일 수 있습니다.)
	// 게임 오버 처리는 일반적으로 Game Mode에서 하지만, 임시로 캐릭터를 제거합니다.
	SetLifeSpan(5.0f); // 5초 후에 액터를 제거합니다.
}