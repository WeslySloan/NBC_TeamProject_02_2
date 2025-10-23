#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "passiveItem.h"
#include "ActiveSkillItem.h"
#include "SkillDataAsset.h"
#include "SkillManager.generated.h"

USTRUCT(BlueprintType)
struct FSkillPool
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Skill")
    bool bIsPassive;

    UPROPERTY(BlueprintReadWrite, Category = "Skill")
    FPassiveItemData PassiveData;

    UPROPERTY(BlueprintReadWrite, Category = "Skill")
    FActiveSkillItemData ActiveData;
};

UCLASS()
class TEAM15CH3PROJECT_API ASkillManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASkillManager();

    // ��ų ��ü ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    TArray<FSkillPool> SkillPool;

    // ������ ���� ���� (�����Ϳ��� �Ҵ�)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    USkillDataAsset* SkillDataAsset;

    virtual void BeginPlay() override;

};
