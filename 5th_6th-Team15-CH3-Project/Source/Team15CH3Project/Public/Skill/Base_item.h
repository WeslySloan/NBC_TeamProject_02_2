
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemInterface.h"
#include "Base_item.generated.h"

class USphereComponent;
UCLASS()
class TEAM15CH3PROJECT_API ABase_item : public AActor, public IItemInterface
{
	GENERATED_BODY()
	
public:
	ABase_item();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USphereComponent* Collision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	UStaticMeshComponent* CastingTimeOut;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	UStaticMeshComponent* CastingTimeIn;

	virtual void ActivateItem(AActor* Acivator)override;
	virtual FName GetIemType() const override;

	virtual void DestroyItem();
};
