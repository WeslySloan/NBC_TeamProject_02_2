// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMade/CharacterStatsComponent.h"

// Sets default values
UCharacterStatsComponent::UCharacterStatsComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

float UCharacterStatsComponent::TakeDamage(float DamageAmount)
{
    if (IsDead())
    {
        return 0.0f;
    }

    CurrentHP -= DamageAmount;

    UE_LOG(LogTemp, Warning, TEXT("[Stats] Damage Taken: %.2f, Current HP: %.2f"), DamageAmount, CurrentHP);

    // ��� üũ
    if (CurrentHP <= 0.0f)
    {
        CurrentHP = 0.0f;
        Die();
    }

    return DamageAmount;
}

// ��� ó�� �Լ� 
void UCharacterStatsComponent::Die()
{
    UE_LOG(LogTemp, Error, TEXT("[Stats] Character has DIED! Notifying UI/AI."));
}