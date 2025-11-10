#include "Skill/Base_item.h"
#include "Components/SphereComponent.h"

ABase_item::ABase_item()
{
	PrimaryActorTick.bCanEverTick = false;
}


void ABase_item::ActivateItem(AActor* Acivator)
{

}
FName ABase_item::GetIemType() const
{
	return ItemType;
}

void ABase_item::DestroyItem()
{
	Destroy();
}


