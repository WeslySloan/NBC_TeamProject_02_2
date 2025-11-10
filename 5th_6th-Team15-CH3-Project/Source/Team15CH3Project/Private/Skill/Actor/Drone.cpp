#include "Skill/Actor/Drone.h"
#include "Kismet/GameplayStatics.h"
#include "AI_Monster/AI_Monsters.h"
#include "PlayerMade/PlayerCharacter.h"

ADrone::ADrone()
{
	PrimaryActorTick.bCanEverTick = true;

    DroneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DroneMesh"));
    SetRootComponent(DroneMesh);

    DroneMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    static ConstructorHelpers::FClassFinder<AMonsterBullet> BulletBP(TEXT("/Game/SKill/BluePrint/BP_DroneBullet.BP_DroneBullet"));
    if (BulletBP.Succeeded())
    {
        BulletClass = BulletBP.Class;
    }
}

void ADrone::BeginPlay()
{
    Super::BeginPlay();
    CurrentAngle = StartAngle;
    LastAttackTime = -AttackInterval;
}

void ADrone::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!OrbitTarget)
    {
        return;
    }

    CurrentAngle += OrbitSpeed * DeltaTime;
    float TotalAngle = StartAngle + CurrentAngle;
    float Radians = FMath::DegreesToRadians(TotalAngle);

    FVector OrbitOffset(FMath::Cos(Radians) * OrbitRadius, FMath::Sin(Radians) * OrbitRadius, 150.f);
    FVector TargetLoc = OrbitTarget->GetActorLocation() + OrbitOffset;
    SetActorLocation(TargetLoc);
}
void ADrone::AttackNearbyEnemies()
{
    if (!OwnerCharacter) return;

    FVector MyLocation = GetActorLocation();
    UWorld* World = GetWorld();
    if (!World) return;

    TArray<AActor*> Monsters;
    UGameplayStatics::GetAllActorsOfClass(World, AAI_Monsters::StaticClass(), Monsters);

    for (AActor* Actor : Monsters)
    {
        AAI_Monsters* Monster = Cast<AAI_Monsters>(Actor);
        if (!Monster || Monster->IsDead()) continue;

        if (FVector::Dist(MyLocation, Monster->GetActorLocation()) <= AttackRadius)
        {
            if (BulletClass)
            {
                FRotator ShootRot = (Monster->GetActorLocation() - MyLocation).Rotation();
                FActorSpawnParameters SpawnParams;
                SpawnParams.Owner = this;
                AMonsterBullet* Bullet = World->SpawnActor<AMonsterBullet>(BulletClass, MyLocation, ShootRot, SpawnParams);
                if (Bullet)
                {
                    Bullet->FireInDirection(ShootRot.Vector());
                }
            }
        }
    }
}

void ADrone::SetOrbitTarget(AActor* Target)
{
    OrbitTarget = Target;
    OwnerCharacter = Cast<APlayerCharacter>(Target);
}
