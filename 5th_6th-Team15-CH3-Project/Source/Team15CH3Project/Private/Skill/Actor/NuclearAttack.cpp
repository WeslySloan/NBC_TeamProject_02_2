#include "Skill/Actor/NuclearAttack.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "Math/UnrealMathUtility.h"
#include "CollisionQueryParams.h" 
#include "CollisionShape.h"

ANuclearAttack::ANuclearAttack()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	DroneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DroneMesh"));
	DroneMesh->SetupAttachment(SceneRoot);

	CastingMeshIn = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GroundMesh"));
	CastingMeshIn->SetupAttachment(SceneRoot);

	CastingMeshOut = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CastingMesh"));
	CastingMeshOut->SetupAttachment(SceneRoot);

	GrowSpeed = 5.0f;
	MaxXScale = 35.0f;
	BaseYScale = 1.0f;
	DamageRadius = 2000.f;
	Damage = 1000.f;
	bExploded = false;

	CurrentXScale = 0.1f;

	HoverSpeed = 7.0f;     // 위아래로 흔들리는 속도
	HoverAmplitude = 30.f; // 위아래로 움직이는 높이
	TimeAccumulator = 0.f; // 시간 누적값
}

void ANuclearAttack::BeginPlay()
{
	Super::BeginPlay();

	TargetLocation = GetActorLocation();

	CastingMeshIn->SetRelativeScale3D(FVector(MaxXScale, MaxXScale, 0.001f));
	CastingMeshOut->SetRelativeScale3D(FVector(CurrentXScale, MaxXScale, 0.002f));
}

void ANuclearAttack::Tick(float DeltaTime)
{
	TimeAccumulator += DeltaTime;
	FVector BaseLocation = DroneMesh->GetRelativeLocation();
	float HoverOffset = FMath::Sin(TimeAccumulator * HoverSpeed) * HoverAmplitude;
	BaseLocation.Z = HoverOffset; //진자운동 호버링 시킬꺼
	DroneMesh->SetRelativeLocation(BaseLocation);

	if (bIsMoving && !bExploded)
	{
		FVector NewLocation = DroneMesh->GetRelativeLocation() + MoveDirection * MoveSpeed * DeltaTime;
		DroneMesh->SetRelativeLocation(NewLocation);

		if (FVector::Dist(DroneMesh->GetComponentLocation(), TargetLocation) < 150.f)
		{
			StopMoving();
		}
	}
	else if (bIsMoving && bExploded)
	{
		FVector NewLocation = DroneMesh->GetRelativeLocation() + MoveDirection * MoveSpeed * DeltaTime;
		DroneMesh->SetRelativeLocation(NewLocation);
	}

	CurrentXScale += GrowSpeed * DeltaTime; // 장판 캐스팅
	CastingMeshOut->SetRelativeScale3D(FVector(CurrentXScale, CurrentXScale, 0.002f));

	if (bShowDamageRadius)
	{
		DrawDebugSphere(
			GetWorld(),
			GetActorLocation(),
			DamageRadius,
			32,
			FColor::Red,
			false,  // 지속시간
			0.f,    // LifeTime
			0,      // DepthPriority
			2.f     // Thickness
		);
	}

	if (CurrentXScale >= MaxXScale)
	{
		Explode();
	}
}

void ANuclearAttack::StopMoving()
{
	bIsMoving = false;
}

void ANuclearAttack::StartMoving()
{
	bIsMoving = true;
}


void ANuclearAttack::Explode()
{
	if (bExploded) return;
	bExploded = true;

	if (CastingMeshIn)
		CastingMeshIn->SetVisibility(false);
	if (CastingMeshOut)
		CastingMeshOut->SetVisibility(false);

	UWorld* World = GetWorld();
	if (!World) return;

	const FVector Origin = GetActorLocation();

	TArray<AActor*> IgnoreActors;
	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0))
	{
		IgnoreActors.Add(PlayerPawn);
	}

	UGameplayStatics::ApplyRadialDamage(
		World,
		Damage,
		Origin,
		DamageRadius,
		UDamageType::StaticClass(),
		IgnoreActors,
		this,
		GetInstigatorController(),
		true
	);


	if (ExplosionEffect)
	{
		for (int32 i = 0; i < 50; ++i) // 파티클 생성 갯수
		{
			float Delay = FMath::RandRange(0.1f, 0.5f);
			FTimerHandle TimerHandle;
			ExplosionTimers.Add(TimerHandle);

			World->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this, World]()
				{
					if (!World || !ExplosionEffect) return;

					float Angle = FMath::RandRange(0.f, 360.f);
					float Radius = FMath::RandRange(0.f, DamageRadius * 0.8f);

					FVector SpawnPos = GetActorLocation() + FVector(FMath::Cos(FMath::DegreesToRadians(Angle)) * Radius,
						FMath::Sin(FMath::DegreesToRadians(Angle)) * Radius,
						50.f);

					UGameplayStatics::SpawnEmitterAtLocation(World, ExplosionEffect, SpawnPos);

				}), Delay, false);
		}
	}
	StartMoving();
	SetLifeSpan(3.0f);
}


