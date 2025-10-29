#include "Skill/SkillUseIndicatorComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/PlayerController.h"
#include "PlayerMade/Controller/PlayerMadeController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

USkillUseIndicatorComponent::USkillUseIndicatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bIsVisible = true; // 시작 시 표시
}

void USkillUseIndicatorComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IndicatorActorClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnedIndicatorActor = GetWorld()->SpawnActor<AActor>(
			IndicatorActorClass,
			GetOwner()->GetActorLocation(),
			FRotator::ZeroRotator,
			SpawnParams
		);

		if (SpawnedIndicatorActor)
		{
			ShowIndicator(); // 게임 시작 시 바로 표시
		}
	}
}

void USkillUseIndicatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsVisible && SpawnedIndicatorActor)
	{
		UpdateIndicatorLocation();
	}
}

void USkillUseIndicatorComponent::ShowIndicator()
{
	if (SpawnedIndicatorActor)
	{
		SpawnedIndicatorActor->SetActorHiddenInGame(false);
		bIsVisible = true;
	}
}

void USkillUseIndicatorComponent::HideIndicator()
{
	if (SpawnedIndicatorActor)
	{
		SpawnedIndicatorActor->SetActorHiddenInGame(true);
		bIsVisible = false;
	}
}

void USkillUseIndicatorComponent::UpdateIndicatorLocation()
{
	if (!bIsVisible || !SpawnedIndicatorActor) return;

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return;

	APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController());
	if (!PC) return;

	FVector MouseWorldOrigin, MouseWorldDirection;
	if (PC->DeprojectMousePositionToWorld(MouseWorldOrigin, MouseWorldDirection))
	{
		// XY는 마우스 위치 그대로, Z는 충분히 높게
		FVector Start(MouseWorldOrigin.X, MouseWorldOrigin.Y, 5000.f);   // 지면 위 5000
		FVector End(MouseWorldOrigin.X, MouseWorldOrigin.Y, -5000.f);    // 지면 아래까지

		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(GetOwner());

		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
		{
			FVector NewLocation = Hit.Location;
			NewLocation.Z += 5.f; // 지면에서 살짝 띄움
			SpawnedIndicatorActor->SetActorLocation(NewLocation);
		}
	}
}