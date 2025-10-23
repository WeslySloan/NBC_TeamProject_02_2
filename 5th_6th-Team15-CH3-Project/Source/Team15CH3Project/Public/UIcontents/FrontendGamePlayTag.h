#pragma once

#include "NativeGameplayTags.h"

namespace FrontendGamePlayTags
{
	//Frontend Widget Stack Tags
	TEAM15CH3PROJECT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_Modal); // Modal (Dialog)
	TEAM15CH3PROJECT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_GameMenu); // In-Game Menu
	TEAM15CH3PROJECT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_GameHud); // Main HUD
	TEAM15CH3PROJECT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_Frontend); // Main Frontend

	//Frontend Widgets
	TEAM15CH3PROJECT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_PressAnyKeyScreen);
	TEAM15CH3PROJECT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_MainMenuScreen);
	TEAM15CH3PROJECT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_ConfirmScreen);
}