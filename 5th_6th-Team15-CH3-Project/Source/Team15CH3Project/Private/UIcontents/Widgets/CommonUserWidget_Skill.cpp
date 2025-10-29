#include "UIcontents/Widgets/CommonUserWidget_Skill.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "GameManager/Pro_M_GameInstance.h"
#include "Kismet/GameplayStatics.h"


void UCommonUserWidget_Skill::NativeConstruct()
{
	Super::NativeConstruct();

	if (UGameInstance* GI = UGameplayStatics::GetGameInstance(GetWorld()))
	{
		if (UPro_M_GameInstance* ProGI = Cast<UPro_M_GameInstance>(GI))
		{
			if (ProGI->SkillDataAsset && ProGI->SkillDataAsset->PassiveSkills.Num() > 0)
			{
				const FPassiveItemData& Skill = ProGI->SkillDataAsset->PassiveSkills[0];

				if (SkillNameText)
				{
					SkillNameText->SetText(Skill.SkillName);
				}
			}
		}
	}
}
