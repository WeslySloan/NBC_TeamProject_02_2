#include "UIcontents/Widgets/Widget_PrimaryLayout.h"
#include "UIContents/FrontendDebugHelper.h"

UCommonActivatableWidgetContainerBase* UWidget_PrimaryLayout::FindRegisterStackByTag(const FGameplayTag& InTag) const
{

	checkf(RegisteredWidgetStackMap.Contains(InTag), TEXT("Can not Find the Widget stack by the tag %s"), *InTag.ToString());

	return RegisteredWidgetStackMap.FindRef(InTag);

}

void UWidget_PrimaryLayout::RegisteredWidgetStack(UPARAM(meta = (Categories = "Frontend.WidgetStack")) FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* InStack)
{
	if (!IsDesignTime()) // 디자인 타임인지 확인
	{
		if (!RegisteredWidgetStackMap.Contains(InStackTag))
		{
			RegisteredWidgetStackMap.Add(InStackTag, InStack);
		}
	}
}

