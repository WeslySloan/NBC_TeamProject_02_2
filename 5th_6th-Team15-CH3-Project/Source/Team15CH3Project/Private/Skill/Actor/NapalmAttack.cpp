#include "Skill/Actor/NapalmAttack.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "Math/UnrealMathUtility.h"
#include "CollisionQueryParams.h" 
#include "CollisionShape.h"

ANapalmAttack::ANapalmAttack()
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

	GrowSpeed = 1.0f;
	MaxXScale = 5.0f;
	BaseYScale = 1.0f;
	DamageRadius = 500.f;
	Damage = 80.f;
	bExploded = false;

	CurrentXScale = 0.1f;
}

void ANapalmAttack::BeginPlay()
{
	Super::BeginPlay();

	CastingMeshIn->SetRelativeScale3D(FVector(MaxXScale, BaseYScale, 0.001f));
	CastingMeshOut->SetRelativeScale3D(FVector(CurrentXScale, BaseYScale, 0.001f));
}

void ANapalmAttack::Tick(float DeltaTime)
{
	if (bExploded) return;

	CurrentXScale += GrowSpeed * DeltaTime; // 장판 캐스팅
	CastingMeshOut->SetRelativeScale3D(FVector(CurrentXScale, BaseYScale, 0.001f));

	if (CurrentXScale >= MaxXScale)
	{
		Explode();
	}
}

void ANapalmAttack::Explode()
{
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
		for (int32 i = 0; i < 20; ++i) // 파티클 생성 갯수
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

