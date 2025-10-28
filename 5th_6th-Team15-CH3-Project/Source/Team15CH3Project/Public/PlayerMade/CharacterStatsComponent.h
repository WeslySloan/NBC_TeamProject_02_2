// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatsComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEAM15CH3PROJECT_API UCharacterStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterStatsComponent();

	// 추가] 데미지 처리 함수 선언
	float TakeDamage(float DamageAmount);

	// [추가] 캐릭터 사망 상태 Getter 함수
	UFUNCTION(BlueprintPure, Category = "Stats | Resources")
	bool IsDead() const { return CurrentHP <= 0.0f; }

protected:
	// [추가] 사망 시 호출되는 내부 함수
	void Die();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Combat")
	float AttackDamage = 10.0f;



	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Combat")
	//float AttackDamageMin = 9;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Combat")
	//float AttackDamageMax = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Combat")
	float AttackSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Combat")
	int32 ProjectileCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Resources")
	float MaxMP = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats | Resources")
	float CurrentMP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Resources")
	float MaxHP = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats | Resources")
	float CurrentHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Misc")
	float MoveSpeed = 600.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats | Misc")
	int32 Experience = 0;
};