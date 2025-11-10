#include "AI_Monster/MonsterBullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerMade/PlayerCharacter.h"



AMonsterBullet::AMonsterBullet()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionComp->InitSphereRadius(15.f);
	SetRootComponent(CollisionComp);

	// 충돌 채널/응답 설정
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComp->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	// 플레이어(펀)과는 Overlap로 맞춤 판정
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	// 월드와는 Block으로 부딪히면 파괴
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	// Hit 이벤트를 받기 위해 필요
	CollisionComp->SetNotifyRigidBodyCollision(true);

	// 이벤트 바인딩
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AMonsterBullet::OnBeginOverlap);
	CollisionComp->OnComponentHit.AddDynamic(this, &AMonsterBullet::OnHit);

	// === Projectile Movement ===
	ProjecttileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjecttileMovementComp->SetUpdatedComponent(CollisionComp);
	ProjecttileMovementComp->InitialSpeed = InitialSpeed;
	ProjecttileMovementComp->MaxSpeed = MaxSpeed;
	ProjecttileMovementComp->bRotationFollowsVelocity = true;
	ProjecttileMovementComp->bShouldBounce = false;
	ProjecttileMovementComp->ProjectileGravityScale = 0.f;


	InitialLifeSpan = 3.0f;
}


void AMonsterBullet::FireInDirection(const FVector& ShootDirection)
{
	ProjecttileMovementComp->Velocity = ShootDirection * ProjecttileMovementComp->InitialSpeed;
}

void AMonsterBullet::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 /*OtherBodyIndex*/,
	bool /*bFromSweep*/,
	const FHitResult& /*SweepResult*/
)
{
	// 자기 자신/소유자 무시
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner())
	{
		return;
	}

	if (APlayerCharacter* PC = Cast<APlayerCharacter>(OtherActor))
	{
		ApplyDamageAndDestroy(PC);
	}
}

void AMonsterBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!OtherActor || OtherActor == this || OtherActor == GetOwner()) return;

    // 데미지 주고 파괴
    UGameplayStatics::ApplyDamage(
        OtherActor,
        Damage,
        GetInstigatorController(),
        this,
        UDamageType::StaticClass()
    );

	if (!OtherActor || OtherActor == this || OtherActor == GetOwner())
	{
		return;
	}
	Destroy();
}

void AMonsterBullet::ApplyDamageAndDestroy(AActor* HitActor)
{
	UGameplayStatics::ApplyDamage(
		HitActor,
		Damage,
		GetInstigatorController(),
		this,
		DamageTypeClass ? *DamageTypeClass : UDamageType::StaticClass()
	);

	Destroy();
}