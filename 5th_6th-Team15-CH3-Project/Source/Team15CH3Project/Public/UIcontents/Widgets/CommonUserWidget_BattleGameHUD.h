#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Skill/PassiveItem.h"
#include "Skill/ActiveSkillItem.h"
#include "Blueprint/UserWidget.h"
#include "UIcontents/Widgets/CommonUserWidget_Skill.h"
#include "Skill/SkillInventoryComponent.h"

// HUD 위젯에서 사용되는 컴포넌트
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

// PlayerCharacter, StatsComponent
#include "PlayerMade/PlayerCharacter.h"
#include "PlayerMade/CharacterStatsComponent.h"

#include "CommonUserWidget_BattleGameHUD.generated.h"


UCLASS()
class TEAM15CH3PROJECT_API UCommonUserWidget_BattleGameHUD : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:

    //==Passive==
    //Image
    UPROPERTY(meta = (BindWidget))
    class UImage* Image_Skill_1;

    UPROPERTY(meta = (BindWidget))
    class UImage* Image_Skill_2;

    UPROPERTY(meta = (BindWidget))
    class UImage* Image_Skill_3;

    UPROPERTY(meta = (BindWidget))
    class UImage* Image_Skill_4;

    //Stack
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* PassiveStackCnt_1;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* PassiveStackCnt_2;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* PassiveStackCnt_3;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* PassiveStackCnt_4;

    UPROPERTY()
    EPassiveItemType SkillSlot_1_Type = EPassiveItemType::None;

    UPROPERTY()
    EPassiveItemType SkillSlot_2_Type = EPassiveItemType::None;

    UPROPERTY()
    EPassiveItemType SkillSlot_3_Type = EPassiveItemType::None;

    UPROPERTY()
    EPassiveItemType SkillSlot_4_Type = EPassiveItemType::None;

    UPROPERTY()
    TArray<EPassiveItemType> AssignedSkillTypesPassive;

    //==Active==
    //Image
    UPROPERTY(meta = (BindWidget))
    class UImage* Image_Skill_5;

    UPROPERTY(meta = (BindWidget))
    class UImage* Image_Skill_6;

    UPROPERTY(meta = (BindWidget))
    class UImage* Image_Skill_7;

    UPROPERTY(meta = (BindWidget))
    class UImage* Image_Skill_8;

    //Stack
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ActiveStackCnt_1;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ActiveStackCnt_2;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ActiveStackCnt_3;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ActiveStackCnt_4;

    UPROPERTY()
    EActiveSkillItemType SkillSlot_1_Active = EActiveSkillItemType::None;

    UPROPERTY()
    EActiveSkillItemType SkillSlot_2_Active = EActiveSkillItemType::None;

    UPROPERTY()
    EActiveSkillItemType SkillSlot_3_Active = EActiveSkillItemType::None;

    UPROPERTY()
    EActiveSkillItemType SkillSlot_4_Active = EActiveSkillItemType::None;

    UPROPERTY()
    TArray<EActiveSkillItemType> AssignedSkillTypesActive;


    //HP & MP & XP 
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthBar;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* HPText;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = "true"))
    float DisplayedHealthPercent = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float HealthDecreaseSpeed = 5.0f;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* MPBar;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* MPText;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MP", Meta = (AllowPrivateAccess = "true"))
    float DisplayedMPPercent = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MP")
    float MPDecreaseSpeed = 5.0f;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* XPBar;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* XPText;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XP", Meta = (AllowPrivateAccess = "true"))
    float DisplayedXpPercent = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XP")
    float XpLerpSpeed = 5.0f;
	//Kill Count
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* KillCountText;

    //플레이어 캐릭터 참조
    UPROPERTY()
    APlayerCharacter* MyPlayerChar;

    //NativeTick 매프레임 업데이트 할 때 사용
    virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

    // NativeConstruct는 초기 설정용으로 사용
    virtual void NativeConstruct() override;

    void ShowSkillSelectUI(UCharacterStatsComponent* StatsComp);

    //초기화
    UFUNCTION(BlueprintCallable)
    void InitHUD();

    //타이머
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* CurrentTime;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game Time")
    float GameTimeElapsed = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UCommonUserWidget_Skill> SkillSelectWidgetClass;

    UPROPERTY()
    USkillInventoryComponent* SkillInventory = nullptr;
 
private:
    //=Kill=
	FTimerHandle KillCountUpdateTimerHandle;
    //주기적으로 호출 될 함수
    void CheckKillCount();
	// UI업데이트 함수
    void UpdateDisplay(int32 NewKillCount);
    int32 KillCount = 0;

    // 스킬 선택
    int32 LastKnownLevel = 0;
    bool bIsSkillSelectUIShown = false;

    //승리 또는 패배 후 딜레이 후 나타나는 UI
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> DelayWidgetClass;
    bool Delay = false;
    FTimerHandle WinLoseDelayTimerHandle;
    UFUNCTION()
    void HandleWinLoseDelay();

    //승리 또는 패배 UI
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> WinLoseWidgetClass;
    bool WinLoseUIShown = false;

    TArray<FString> String = {
        TEXT("The stars say nothing; they merely shine."),
        TEXT("The universe is vast, but meaning is always made by humans."),
        TEXT("A signal came in the silence, and then nothing happened."),
        TEXT("Life is not a calculation, but the end was always precise."),
        TEXT("An old satellite is still repeatedly transmitting a 'return home' signal."),
        TEXT("We found God, but it was just an ancient algorithm."),
        TEXT("The Earth is gone, and we still call out names."),
        TEXT("Even in a vacuum, memory remains. It is the only evidence of life."),
        TEXT("Time is not a straight line; only we believe it to be."),
        TEXT("The last human looked at the stars and powered down.")
    };

};