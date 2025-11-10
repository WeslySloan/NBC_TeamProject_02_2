#include "Skill/Actor/ShootingLasers.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Skill/ActiveSkillItem.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "Math/UnrealMathUtility.h"
#include "CollisionQueryParams.h" 
#include "CollisionShape.h"

AShootingLasers::AShootingLasers()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetupAttachment(SceneRoot);
	CollisionComp->InitSphereRadius(0.1f);

	CastingMeshIn = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GroundMesh"));
	CastingMeshIn->SetupAttachment(CollisionComp);

	CastingMeshOut = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CastingMesh"));
	CastingMeshOut->SetupAttachment(CollisionComp);

	GrowSpeed = 1.8f;
	MaxScale = 5.0f;
	BaseScale = 1.0f;
	DamageRadius = 500.f;
	Damage = 80.f;
	bIsLaserShot = false;

	CurrentScale = 0.1f;
}

void AShootingLasers::BeginPlay()
{
	Super::BeginPlay();

	CastingMeshIn->SetRelativeScale3D(FVector(MaxScale, MaxScale, 0.001f));
	CastingMeshOut->SetRelativeScale3D(FVector(CurrentScale, CurrentScale, 0.001f));
}

void AShootingLasers::Tick(float DeltaTime)
{
	if (bIsLaserShot) return;

	CurrentScale += GrowSpeed * DeltaTime;
	CastingMeshOut->SetRelativeScale3D(FVector(CurrentScale, CurrentScale, 0.001f));

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

	if (CurrentScale >= MaxScale)
	{
		Shoting();
	}
}

void AShootingLasers::Shoting()
{
	bIsLaserShot = true;

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
		for (int32 i = 0; i < 50; ++i)
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

	SetLifeSpan(2.f);
}