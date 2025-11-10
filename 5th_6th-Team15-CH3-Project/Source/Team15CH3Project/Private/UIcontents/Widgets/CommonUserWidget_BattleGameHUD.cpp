#include "UIcontents/Widgets/CommonUserWidget_BattleGameHUD.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Blueprint/WidgetTree.h"
#include "PlayerMade/PlayerCharacter.h"
#include "PlayerMade/CharacterStatsComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "UIcontents/Widgets/CommonUserWidget_Skill.h"
#include "AI_Monster/AI_Monsters.h"
#include "TimerManager.h"
#include "Engine/World.h"

void UCommonUserWidget_BattleGameHUD::InitHUD()
{
	//캐릭터 가져오기 & 이걸 이젠 InitHUD를 HUD블루프린트로 연결 
	if (APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		MyPlayerChar = PlayerChar;

		if (UCharacterStatsComponent* StatsComp = PlayerChar->FindComponentByClass<UCharacterStatsComponent>())
		{
			LastKnownLevel = StatsComp->Level; //시작 시점 레벨 저장
			UE_LOG(LogTemp, Warning, TEXT("[HUD] InitHUD - Starting Level: %d"), LastKnownLevel);
		}
	}
}

void UCommonUserWidget_BattleGameHUD::CheckKillCount()
{

	if (AAI_Monsters::GetTotalKillCount() != KillCount)
	{
		KillCount = AAI_Monsters::GetTotalKillCount();
		UpdateDisplay(KillCount); //UI 갱신
	}
}

void UCommonUserWidget_BattleGameHUD::UpdateDisplay(int32 NewKillCount)
{
	if (KillCountText)
	{
		//바인딩된 UTextBlock에 킬 카운트 텍스트 적용
		FText DisplayText = FText::Format(FText::FromString(TEXT("KILLS: {0}")), FText::AsNumber(NewKillCount));
		KillCountText->SetText(DisplayText);
	}
}
//HUD 위젯이 생성되고 초기화될 때 호출되는 함수
void UCommonUserWidget_BattleGameHUD::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateDisplay(KillCount); //HUD에 생성될 때 즉시 화면에 킬 카운트 표시, 0.3초의 공백을 방지

	//변경 감지 킬 카운트 갱신
	GetWorld()->GetTimerManager().SetTimer(
		KillCountUpdateTimerHandle,
		this,
		&UCommonUserWidget_BattleGameHUD::CheckKillCount,
		0.3f,
		true
	);
}

void UCommonUserWidget_BattleGameHUD::ShowSkillSelectUI(UCharacterStatsComponent* StatsComp)
{

	// 위젯 생성
	UCommonUserWidget_Skill* SkillSelectUI = CreateWidget<UCommonUserWidget_Skill>(GetWorld(), SkillSelectWidgetClass);

	//정상 생성
	UE_LOG(LogTemp, Warning, TEXT("[HUD] SkillSelectUI successfully created!"));
	SkillSelectUI->AddToViewport(100);
	bIsSkillSelectUIShown = true;

	//게임 일시정지 및 입력 전환
	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->SetPause(true);
		PC->SetShowMouseCursor(true);
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(SkillSelectUI->TakeWidget());
		PC->SetInputMode(InputMode);
	}
}


void UCommonUserWidget_BattleGameHUD::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	if (!MyPlayerChar) return;

	//UCharacterStatsComponent에서 스테이트 가져오기
	UCharacterStatsComponent* StatsComp = MyPlayerChar->FindComponentByClass<UCharacterStatsComponent>();
	if (!StatsComp) return;

	float CurrentHP = StatsComp->CurrentHP; 
	float MaxHP = StatsComp->MaxHP; 

	float CurrentMP = StatsComp->CurrentMP;
	float MaxMP = StatsComp->MaxMP;

	int32 Level = StatsComp->Level;

	int32 CurrentXP = StatsComp->Experience;
	int32 MaxXp = StatsComp->MaxExperience;

	if (Level != LastKnownLevel && Level > 0)
	{
		if (Level > LastKnownLevel)
		{
			UE_LOG(LogTemp, Warning, TEXT("Level Up! %d -> %d"), LastKnownLevel, Level);
			ShowSkillSelectUI(StatsComp);
		}

		LastKnownLevel = Level; // 감지 후에 갱신
	}

	if (XPBar != nullptr)
	{
		const float TargetXpPercent = (MaxXp > 0) ? (static_cast<float>(CurrentXP) / MaxXp) : 0.0f;

		DisplayedXpPercent = FMath::FInterpTo(
			DisplayedXpPercent,            
			TargetXpPercent,               
			DeltaTime,                     
			XpLerpSpeed                    
		);

		XPBar->SetPercent(DisplayedXpPercent);
	}

	if (XPText)
	{
		FString LevelString = FString::Printf(TEXT("Lv.%d"), Level);
		XPText->SetText(FText::FromString(LevelString));
	}

	//바인딩 값을 매 프레임마다 갱신
	const float TargetHPPercent = (MaxHP > 0.f) ? (CurrentHP / MaxHP) : 0.f;

	if (HealthBar != nullptr)
	{
		//FInterpTo를 사용하여 부드럽게 보간, DisplayedHealthPercent를 TargetPercent를 향해 천천히 이동시킵니다.
		DisplayedHealthPercent = FMath::FInterpTo(
			DisplayedHealthPercent,        // 현재 표시 값 (시작)
			TargetHPPercent,                 // 실제 HP 퍼센트 값 (목표)
			DeltaTime,                     // GetWorld()->GetDeltaSeconds()와 동일 (프레임 시간)
			HealthDecreaseSpeed            // 보간 속도 (헤더에서 정의된 변수)
		);

		HealthBar->SetPercent(DisplayedHealthPercent);
	}

	if (HPText)
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentHP, MaxHP)));

	const float TargetMPPercent = (MaxMP > 0.f) ? (CurrentMP / MaxMP) : 0.f;

	if (MPBar != nullptr)
	{
		DisplayedMPPercent = FMath::FInterpTo(
			DisplayedMPPercent,    
			TargetMPPercent,
			DeltaTime,                     
			MPDecreaseSpeed
		);

		MPBar->SetPercent(DisplayedMPPercent);
	}

	if (MPText)
		MPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentMP, MaxMP)));

	if (KillCount >= 1000)
	{
		if (WinLoseUIShown)
		{
			return;
		}

		if (WinLoseWidgetClass)
		{

			UUserWidget* WinLoseUI = CreateWidget<UUserWidget>(GetWorld(), WinLoseWidgetClass);

			if (WinLoseUI)
			{
				//텍스트 변경
				if (UTextBlock* WinLoseText = Cast<UTextBlock>(WinLoseUI->GetWidgetFromName(TEXT("CommonTextBlock_WinLose"))))
				{
					WinLoseText->SetText(FText::FromString(TEXT("Win")));
				}

				WinLoseUI->AddToViewport();
				WinLoseUIShown = true;

				GetWorld()->GetTimerManager().SetTimer(
					WinLoseDelayTimerHandle,
					this,
					&UCommonUserWidget_BattleGameHUD::HandleWinLoseDelay,
					1.5f, // 3초 후 Delay UI 표시 및 게임 정지
					false
				);
			}
		}
		return;
	}

	if (CurrentHP > 0.0f)
	{
		//경과 시간 누적
		GameTimeElapsed += DeltaTime;

		//시간 업데이트
		if (CurrentTime)
		{
			int32 TotalSeconds = FMath::FloorToInt(GameTimeElapsed);
			int32 Minutes = TotalSeconds / 60;
			int32 Seconds = TotalSeconds % 60;

			//"00:00" 형태로 포맷팅합니다.
			FString TimeString = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
			CurrentTime->SetText(FText::FromString(TimeString));
		}
	}
	else//(CurrentHP <= 0.0f 일 때)
	{
		if (WinLoseUIShown) //이미 UI가 표시되었으면 반환
		{
			return;
		}

		if (WinLoseWidgetClass) //UI 클래스가 설정되어 있으면
		{

			UUserWidget* WinLoseUI = CreateWidget<UUserWidget>(GetWorld(), WinLoseWidgetClass);
			
			if (WinLoseUI)
			{
				//텍스트 변경
				if (UTextBlock* WinLoseText = Cast<UTextBlock>(WinLoseUI->GetWidgetFromName(TEXT("CommonTextBlock_WinLose"))))
				{
					WinLoseText->SetText(FText::FromString(TEXT("LOSE")));
				}

				WinLoseUI->AddToViewport();
				WinLoseUIShown = true;

				GetWorld()->GetTimerManager().SetTimer(
					WinLoseDelayTimerHandle,
					this,
					&UCommonUserWidget_BattleGameHUD::HandleWinLoseDelay,
					1.5f, // 3초 후 Delay UI 표시 및 게임 정지
					false
				);
			}
		}
		return;
	}
}

void UCommonUserWidget_BattleGameHUD::HandleWinLoseDelay()
{
	AAI_Monsters::ResetTotalKillCount();

	//DelayWidgetClass를 생성
	if (DelayWidgetClass)
	{
		UUserWidget* DelayUI = CreateWidget<UUserWidget>(GetWorld(), DelayWidgetClass);
		if (DelayUI)
		{
			DelayUI->AddToViewport(10000);
			Delay = true;
		}
	}
	GetWorld()->GetTimerManager().ClearTimer(WinLoseDelayTimerHandle);
}