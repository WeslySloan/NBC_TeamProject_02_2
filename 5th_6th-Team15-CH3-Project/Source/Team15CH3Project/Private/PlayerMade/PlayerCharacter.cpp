// PlayerCharacter.cpp

#include "PlayerMade/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "PlayerMade/CharacterStatsComponent.h"
#include "PlayerMade/AutoAttackComponent.h"
#include "Components/CapsuleComponent.h"
#include "PlayerMade/AutoAttackComponent.h"
#include "Animation/AnimMontage.h"
#include "Skill/SkillInventoryComponent.h"
#include "Skill/SkillUseIndicatorComponent.h"

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

	AutoAttackComponent = CreateDefaultSubobject<UAutoAttackComponent>(TEXT("AutoAttack"));

	SkillInventory = CreateDefaultSubobject<USkillInventoryComponent>(TEXT("SkillInventory"));

	SkillUseIndicator = CreateDefaultSubobject<USkillUseIndicatorComponent>(TEXT("SkillUseIndicator"));

	// === 2. 이동 및 회전 설정 ===
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// 회전 속도 제한 (보간) - 낮을수록 부드럽게 회전
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	// 가속도를 최대로 올려 즉각 이동
	GetCharacterMovement()->MaxAcceleration = 99999.0f;

	// 감속도를 최대로 올려 즉각 정지
	GetCharacterMovement()->BrakingDecelerationWalking = 99999.0f;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// BeginPlay에서 MaxWalkSpeed 설정
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
		if (IA_LeftClick)
		{
			EnhancedInputComponent->BindAction(IA_LeftClick, ETriggerEvent::Started, this, &APlayerCharacter::OnLeftClick);
		}
		if (SkillQAction)
		{
			EnhancedInputComponent->BindAction(SkillQAction, ETriggerEvent::Started, this, &APlayerCharacter::SkillQ);
		}
		if (SkillEAction)
		{
			EnhancedInputComponent->BindAction(SkillEAction, ETriggerEvent::Started, this, &APlayerCharacter::SkillE);
		}
		if (SkillRAction)
		{
			EnhancedInputComponent->BindAction(SkillRAction, ETriggerEvent::Started, this, &APlayerCharacter::SkillR);
		}
		if (SkillCAction)
		{
			EnhancedInputComponent->BindAction(SkillCAction, ETriggerEvent::Started, this, &APlayerCharacter::SkillC);
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
		//const FRotator Rotation = CameraBoom->GetComponentRotation();
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
			// 직접 회전 코드를 제거하여 CharacterMovementComponent의 RotationRate를 따르게 함
			//SetActorRotation(MoveDir.Rotation());
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
		//	const FVector RightDirection = FRotator(YawRotation).GetUnitAxis(EAxis::Y);
		//	AddMovementInput(RightDirection, MovementVector.X);
		//}
	}
}

void APlayerCharacter::OnLeftClick(const FInputActionValue& Value)
{
	if (!bIsSKillIndicatorActive || !SkillUseIndicator || !SelectedActiveSkillClass || !StatsComponent)
		return;

	const FName SkillName = SelectedActiveSkill.ActiveItemClass
		? SelectedActiveSkill.ActiveItemClass->GetFName()
		: FName("UnknownSkill");

	// === 쿨타임 관리용 static 맵 ===
	static TMap<FName, bool> SkillCooldownMap;
	static TMap<FName, FTimerHandle> SkillCooldownTimerMap;

	// 쿨타임 중이면 리턴
	if (SkillCooldownMap.FindRef(SkillName))
	{
		UE_LOG(LogTemp, Warning, TEXT("[Skill] %s is on cooldown!"), *SkillName.ToString());
		return;
	}

	// === 마나 확인 ===
	if (StatsComponent->CurrentMP < SelectedActiveSkill.UseMana)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Skill] Not enough MP for %s! (%d / %d)"),
			*SkillName.ToString(),
			(int32)StatsComponent->CurrentMP,
			(int32)SelectedActiveSkill.UseMana);
		return;
	}

	// === 마나 소모 ===
	StatsComponent->CurrentMP -= SelectedActiveSkill.UseMana;
	UE_LOG(LogTemp, Warning, TEXT("[Skill] %s used (Mana -%d, Remain: %d)"),
		*SkillName.ToString(),
		(int32)SelectedActiveSkill.UseMana,
		(int32)StatsComponent->CurrentMP);

	// === 쿨타임 시작 ===
	SkillCooldownMap.Add(SkillName, true);

	FTimerHandle& CooldownTimer = SkillCooldownTimerMap.FindOrAdd(SkillName);

	GetWorld()->GetTimerManager().SetTimer(
		CooldownTimer,
		[this, SkillName]()
		{
			// 쿨타임 종료 처리
			if (UWorld* World = GetWorld())
			{
				World->GetTimerManager().ClearTimer(SkillCooldownTimerMap[SkillName]);
				SkillCooldownTimerMap.Remove(SkillName);
			}
			SkillCooldownMap.Add(SkillName, false);
			UE_LOG(LogTemp, Warning, TEXT("[Skill] %s cooldown ended."), *SkillName.ToString());
		},
		SelectedActiveSkill.CoolTime,
		false
	);

	// === 스킬 스폰 ===
	FVector SpawnLocation = SkillUseIndicator->SpawnedIndicatorActor->GetActorLocation();
	SpawnLocation.Z += 10.0f;
	FRotator SpawnRotation = SkillUseIndicator->SpawnedIndicatorActor->GetActorRotation();

	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.Instigator = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActiveSkillItem* SpawnedSkill = GetWorld()->SpawnActor<AActiveSkillItem>(
		SelectedActiveSkillClass, SpawnLocation, SpawnRotation, Params);

	if (SpawnedSkill && SelectedActiveSkillClass)
	{
		SpawnedSkill->ActiveType = SelectedActiveSkill.Type;
		SpawnedSkill->ActiveSkillData = SelectedActiveSkill;
		SpawnedSkill->ActiveSkillApply(this);
	}

	// === 인디케이터 숨김 ===
	if (USkillUseIndicatorComponent* Indicator = FindComponentByClass<USkillUseIndicatorComponent>())
	{
		Indicator->HideIndicator();
	}
	bIsSKillIndicatorActive = false;
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
	if (StatsComponent->IsDead() && GetLifeSpan() == 0.0f)
	{
		PlayerIsDead();
	}

	return ActualDamage;
}

void APlayerCharacter::PlayerIsDead()
{
	UE_LOG(LogTemp, Error, TEXT("PLAYER IS DEAD! Deactivating input and collision."));
	Dead = true;

	if (DeathMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		// 몽타주를 재생하여 Death_Forward 애니메이션을 즉시 출력
		GetMesh()->GetAnimInstance()->Montage_Play(DeathMontage, 1.0f);
	}

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

	if (AutoAttackComponent)
	{
		AutoAttackComponent->StopAutoAttack();
	}

}

void APlayerCharacter::AddExp(int32 Amount)
{
	if (UCharacterStatsComponent* Stats = FindComponentByClass<UCharacterStatsComponent>())
	{
		Stats->GainExperience(Amount);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[PlayerCharacter] CharacterStatsComponent를 찾을 수 없습니다."));
	}
}

// 스킬 콜백 함수 구현
void APlayerCharacter::SkillQ()
{
	// 스킬 Q 로직
	UE_LOG(LogTemp, Warning, TEXT("Skill Q activated!"));

	if (SkillInventory && SkillInventory->ActiveSkillsInv.Num() > 0)
	{
		// 0번째 스킬 선택
		SelectedActiveSkill = SkillInventory->ActiveSkillsInv[0];
		SelectedActiveSkillClass = SelectedActiveSkill.ActiveItemClass;

		// 인디케이터 켜기
		if (USkillUseIndicatorComponent* SkillIndicator = FindComponentByClass<USkillUseIndicatorComponent>())
		{
			SkillIndicator->ShowIndicator();
			bIsSKillIndicatorActive = true;
		}
	}

}

void APlayerCharacter::SkillE()
{
	// 스킬 E 로직
	UE_LOG(LogTemp, Warning, TEXT("Skill E activated!"));

	if (SkillInventory && SkillInventory->ActiveSkillsInv.Num() > 0)
	{
		// 1번째 스킬 선택
		SelectedActiveSkill = SkillInventory->ActiveSkillsInv[1];
		SelectedActiveSkillClass = SelectedActiveSkill.ActiveItemClass;

		// 인디케이터 켜기
		if (USkillUseIndicatorComponent* SkillIndicator = FindComponentByClass<USkillUseIndicatorComponent>())
		{
			SkillIndicator->ShowIndicator();
			bIsSKillIndicatorActive = true;
		}
	}
}

void APlayerCharacter::SkillR()
{
	// 스킬 R 로직
	UE_LOG(LogTemp, Warning, TEXT("Skill R activated!"));

	if (SkillInventory && SkillInventory->ActiveSkillsInv.Num() > 0)
	{
		// 2번째 스킬 선택
		SelectedActiveSkill = SkillInventory->ActiveSkillsInv[2];
		SelectedActiveSkillClass = SelectedActiveSkill.ActiveItemClass;

		// 인디케이터 켜기
		if (USkillUseIndicatorComponent* SkillIndicator = FindComponentByClass<USkillUseIndicatorComponent>())
		{
			SkillIndicator->ShowIndicator();
			bIsSKillIndicatorActive = true;
		}
	}
}

void APlayerCharacter::SkillC()
{
	// 궁극기 C 로직
	UE_LOG(LogTemp, Warning, TEXT("Skill C activated!"));

	if (SkillInventory && SkillInventory->ActiveSkillsInv.Num() > 0)
	{
		// 3번째 스킬 선택
		SelectedActiveSkill = SkillInventory->ActiveSkillsInv[3];
		SelectedActiveSkillClass = SelectedActiveSkill.ActiveItemClass;

		// 인디케이터 켜기
		if (USkillUseIndicatorComponent* SkillIndicator = FindComponentByClass<USkillUseIndicatorComponent>())
		{
			SkillIndicator->ShowIndicator();
			bIsSKillIndicatorActive = true;
		}
	}
}
// ------------------------------------