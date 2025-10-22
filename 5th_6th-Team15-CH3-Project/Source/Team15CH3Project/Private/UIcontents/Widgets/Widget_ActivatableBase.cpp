#include "UIcontents/Widgets/Widget_ActivatableBase.h"
#include "PlayerMade/Controller/PlayerMadeController.h"

APlayerMadeController* UWidget_ActivatableBase::GetOwningAPlayerMadeController()
{
	if (!CachedOwningFrontendPC.IsValid())
	{
		CachedOwningFrontendPC = GetOwningPlayer<APlayerMadeController>();
	}

	return CachedOwningFrontendPC.IsValid() ? CachedOwningFrontendPC.Get() : nullptr;
}
