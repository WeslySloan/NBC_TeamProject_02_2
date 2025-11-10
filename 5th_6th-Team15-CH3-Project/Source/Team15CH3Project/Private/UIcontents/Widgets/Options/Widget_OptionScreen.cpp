#include "UIcontents/Widgets/Options/Widget_OptionScreen.h"
#include "Input/CommonUIInputTypes.h"
#include "ICommonInputModule.h"

//void UWidget_OptionScreen::NativeOnInitialized()
//{
//	Super::NativeOnInitialized();
//
//	f(!ResetAction.IsNull())
//	{
//		ResetActionHandle = RegisterUIActionBinding(
//			FBindUIActionArgs(
//				ResetAction,
//				true,
//				FSimpleDelegate::CreateUObject(this, &ThisClass::OnResetBoundActionTriggered)
//			)
//		);
//	}
//
//	RegisterUIActionBinding(
//		FBindUIActionArgs(
//			ICommonInputModule::GetSettings().GetDefaultBackAction(),
//			true,
//			FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionTriggered)
//		)
//	);
//
//}
//
//void UWidget_OptionScreen::OnResetBoundActionTriggered()
//{
//
//}
//
//void UWidget_OptionScreen::OnBackBoundActionTriggered()
//{
//	DeactivateWidget();
//}