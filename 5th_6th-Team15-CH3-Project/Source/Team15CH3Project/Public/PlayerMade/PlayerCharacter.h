// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerMade/CharacterStatsComponent.h"
#include "Skill/ActiveSkillItem.h"
#include "PlayerCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UAutoAttackComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UAnimMontage;
class USkillInventoryComponent;
class USkillUseIndicatorComponent;

UCLASS()
class TEAM15CH3PROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool Dead = false;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAutoAttackComponent* AutoAttackComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCharacterStatsComponent* StatsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkillInventoryComponent* SkillInventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkillUseIndicatorComponent* SkillUseIndicator;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;

	// 스킬 인풋 액션 선언부

	bool bIsSKillIndicatorActive = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Skills")
	class UInputAction* IA_LeftClick;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Skills")
	class UInputAction* SkillQAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Skills")
	class UInputAction* SkillEAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Skills")
	class UInputAction* SkillRAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Skills")
	class UInputAction* SkillCAction;
	// ------------------------------------

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	class UAnimMontage* DeathMontage;

	UPROPERTY()
	FActiveSkillItemData SelectedActiveSkill;

	// 선택된 액티브 스킬 클래스
	UPROPERTY()
	TSubclassOf<AActiveSkillItem> SelectedActiveSkillClass;

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);

	void PlayerIsDead();

	// 스킬 콜백 함수 선언부
	void OnLeftClick(const FInputActionValue& Value);

	void SkillQ();
	void SkillE();
	void SkillR();
	void SkillC();
	// ------------------------------------

	void AddExp(int32 Amount);
};