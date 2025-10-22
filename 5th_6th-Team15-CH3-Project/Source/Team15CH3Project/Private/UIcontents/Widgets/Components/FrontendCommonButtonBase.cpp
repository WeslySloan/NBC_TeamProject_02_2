#include "UIcontents/Widgets/Components/FrontendCommonButtonBase.h"
#include "PlayerMade/Controller/PlayerMadeController.h"
#include "CommonTextBlock.h"

void UFrontendCommonButtonBase::SetButtonText(FText InText)
{
	if (CommonTextBlock_ButtonText && !InText.IsEmpty())
	{
		CommonTextBlock_ButtonText->SetText(bUserUpperCaseForButtonText ? InText.ToUpper() : InText);
	}
}

void UFrontendCommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetButtonText(ButtonDisplayText);
}
