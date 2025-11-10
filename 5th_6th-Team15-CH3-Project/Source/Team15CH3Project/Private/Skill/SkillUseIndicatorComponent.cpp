#include "Skill/SkillUseIndicatorComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/PlayerController.h"
#include "PlayerMade/Controller/PlayerMadeController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

USkillUseIndicatorComponent::USkillUseIndicatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bIsVisible = false; // 테스트용으로 일단 바로 키게할꺼
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
			HideIndicator();
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

	APlayerController* PlayerController = Cast<APlayerController>(OwnerPawn->GetController());
	if (!PlayerController) return;

	FVector MouseWorldOrigin, MouseWorldDirection;
	if (!PlayerController->DeprojectMousePositionToWorld(MouseWorldOrigin, MouseWorldDirection))
		return;

	// 1) 교차할 "기준 평면" 높이 (보통 캐릭터의 바닥 Z 또는 레벨의 기본 바닥 Z)
	//    캐릭터 바로 아래 바닥을 원하면 OwnerPawn->GetActorLocation().Z - offset(예: 100)
	float GroundZ = OwnerPawn->GetActorLocation().Z - 5.0f; // 또는 0.f 등 상황에 맞게 조정

	// 2) 마우스 레이와 수평면 Z = GroundPlaneZ 의 교차시간 t 구하기
	//    origin + dir * t 의 Z == GroundPlaneZ -> t = (GroundPlaneZ - origin.Z) / dir.Z
	//    dir.Z 이 거의 0이면(레이가 평행) fallback 처리
	const float EPS = KINDA_SMALL_NUMBER;
	FVector approxXYPoint; // 마우스가 가리키는 대략적인 XY 점 (Z는 GroundPlaneZ)
	bool bHavePlaneIntersection = false;

	if (FMath::Abs(MouseWorldDirection.Z) > EPS)
	{
		float t = (GroundZ - MouseWorldOrigin.Z) / MouseWorldDirection.Z;
		// t가 음수면 카메라 뒤로 가는 점 -> fallback: use origin + dir * someDist
		if (t > 0.f)
		{
			FVector planePoint = MouseWorldOrigin + MouseWorldDirection * t;
			approxXYPoint = FVector(planePoint.X, planePoint.Y, GroundZ);
			bHavePlaneIntersection = true;
		}
	}

	// fallback: 방향이 거의 수평이거나 t<=0 인 경우, 그냥 origin을 XY로 사용
	if (!bHavePlaneIntersection)
	{
		// 일정 거리만큼 전방을 사용해서 XY 얻기
		const float FallbackDist = 3000.f;
		FVector fallbackPoint = MouseWorldOrigin + MouseWorldDirection * FallbackDist;
		approxXYPoint = FVector(fallbackPoint.X, fallbackPoint.Y, GroundZ);
	}

	// 3) approxXYPoint의 위쪽에서 아래쪽으로 안정적 다운 트레이스(지면 찾기)
	FVector TraceStart = FVector(approxXYPoint.X, approxXYPoint.Y, GroundZ + 2000.f); // 충분히 위
	FVector TraceEnd = FVector(approxXYPoint.X, approxXYPoint.Y, GroundZ - 2000.f); // 충분히 아래

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	if (SpawnedIndicatorActor)
		Params.AddIgnoredActor(SpawnedIndicatorActor);

	// 또한 소유자와 스폰된 인디케이터의 모든 프리미티브 컴포넌트 제외 (자기충돌 완전 차단)
	TArray<UPrimitiveComponent*> OwnerComps;
	GetOwner()->GetComponents<UPrimitiveComponent>(OwnerComps);
	for (UPrimitiveComponent* Comp : OwnerComps)
		if (Comp) Params.AddIgnoredComponent(Comp);

	if (SpawnedIndicatorActor)
	{
		TArray<UPrimitiveComponent*> IndComps;
		SpawnedIndicatorActor->GetComponents<UPrimitiveComponent>(IndComps);
		for (UPrimitiveComponent* Comp : IndComps)
			if (Comp) Params.AddIgnoredComponent(Comp);
	}

	// (디버그) DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 0.02f, 0, 1.0f);

	if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, Params))
	{
		FVector NewLocation = Hit.Location;
		NewLocation.Z += 2.5f; // 살짝 띄움
		SpawnedIndicatorActor->SetActorLocation(NewLocation);
	}
	else
	{
		// 아무 것도 맞지 않으면 fallback 위치: approxXYPoint에서 일정 거리 전방
		FVector FallbackLocation = FVector(approxXYPoint.X, approxXYPoint.Y, GroundZ + 200.f);
		SpawnedIndicatorActor->SetActorLocation(FallbackLocation);
	}
}