// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerMade/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"


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

    // === 2. 이동 및 회전 설정 ===
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

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
        const FRotator Rotation = CameraBoom->GetComponentRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        if (!FMath::IsNearlyZero(MovementVector.Y))
        {
            const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
            AddMovementInput(ForwardDirection, MovementVector.Y);
        }

        if (!FMath::IsNearlyZero(MovementVector.X))
        {
            const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
            AddMovementInput(RightDirection, MovementVector.X);
        }
    }
}