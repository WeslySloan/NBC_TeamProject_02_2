#pragma once

#include "CoreMinimal.h"
#include "UIcontents/Widgets/Widget_ActivatableBase.h"
#include "Widget_OptionScreen.generated.h"

UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class TEAM15CH3PROJECT_API UWidget_OptionScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()
	
//protected:
//
//	//~ Begin UUserWidget Interface
//	virtual void NativeOnInitialized() override; // 초기화 시 기본
//	//~ End UUserWidget Interface
//
//private:
//	void OnResetBoundActionTriggered();
//	void OnBackBoundActionTriggered();
//
//	UPROPERTY(EditDefaultsOnly, Category = "Frontend Options Screen", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
//	FDataTableRowHandle ResetAction; //선택된 설정을 재설정할 수 있는 버튼
//
//	FUIActionBindingHandle ResetActionHandle;

};
