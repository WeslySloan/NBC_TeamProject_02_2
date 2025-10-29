#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Skill/passiveItem.h"
#include "Skill/ActiveSkillItem.h"
#include "Skill/SkillDataAsset.h"
#include "CommonUserWidget_Skill.generated.h"



UCLASS()
class TEAM15CH3PROJECT_API UCommonUserWidget_Skill : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* SkillNameText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* SkillDescriptionText;

    UPROPERTY(meta = (BindWidget))
    class UImage* SkillIcon;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* StackText;


};
