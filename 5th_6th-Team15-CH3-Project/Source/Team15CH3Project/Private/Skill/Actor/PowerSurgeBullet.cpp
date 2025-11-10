#include "Skill/Actor/PowerSurgeBullet.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AI_Monster/AI_Monsters.h"
#include "Skill/SkillUseIndicatorComponent.h"
APowerSurgeBullet::APowerSurgeBullet()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
    CollisionComp->InitSphereRadius(30.f);
    CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APowerSurgeBullet::OnOverlapBegin);
    CollisionComp->SetGenerateOverlapEvents(true);
    RootComponent = CollisionComp;

   /* CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AShotgunEffectActor::OnOverlapBegin);*/

    ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
    ParticleComp->SetupAttachment(RootComponent);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = 1500.f;
    ProjectileMovement->MaxSpeed = 1500.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->ProjectileGravityScale = 0.0f;

    InitialLifeSpan = 2.0f;
}

void APowerSurgeBullet::BeginPlay()
{
    //수정이 필요한상태 스킬을 시전하기전 인디케이터 위치값을 받아오는지 테스트 필요함
	Super::BeginPlay();

    AActor* MyOwner = GetOwner();
    FVector FireDir = FVector::ZeroVector;

    if (MyOwner)
    {
        USkillUseIndicatorComponent* IndicatorComp = MyOwner->FindComponentByClass<USkillUseIndicatorComponent>();
        if (IndicatorComp && IndicatorComp->GetSpawnedIndicatorActor()) 
        {
            FVector TargetLoc = IndicatorComp->GetSpawnedIndicatorActor()->GetActorLocation();
            FireDir = (TargetLoc - GetActorLocation()).GetSafeNormal();
        }
        else
        {
            
            FireDir = MyOwner->GetActorForwardVector();
        }
    }
    else
    {
        FireDir = GetActorForwardVector();
    }

    if (!FireDir.IsNearlyZero() && ProjectileMovement)
    {
        ProjectileMovement->Velocity = FireDir * ProjectileMovement->InitialSpeed;
    }
}


void APowerSurgeBullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == GetOwner())
        return;

    if (AAI_Monsters* Monster = Cast<AAI_Monsters>(OtherActor))
    {
        UGameplayStatics::ApplyDamage(Monster, Damage, GetInstigatorController(), this, nullptr);
        UE_LOG(LogTemp, Warning, TEXT("PowerSurgeBullet hit %s! Damage: %.1f"), *Monster->GetName(), Damage);
    }

    Destroy();
}

