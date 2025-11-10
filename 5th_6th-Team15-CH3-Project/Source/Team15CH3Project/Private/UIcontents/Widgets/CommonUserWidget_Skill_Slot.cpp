#include "UIcontents/Widgets/CommonUserWidget_Skill_Slot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "GameManager/Pro_M_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UIcontents/Widgets/CommonUserWidget_BattleGameHUD.h"
#include "UIcontents/Widgets/CommonUserWidget_Skill.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Skill/SkillInventoryComponent.h"
#include "PlayerMade/CharacterStatsComponent.h"
#include "PlayerMade/PlayerCharacter.h"
#include "Skill/passiveItem.h"
#include "GameFramework/CharacterMovementComponent.h"

int32 UCommonUserWidget_Skill_Slot::LastAssignedSlotIndex = 0;
float UCommonUserWidget_Skill_Slot::StaticBaseAttackDamage = 0.0f;

void UCommonUserWidget_Skill_Slot::NativeConstruct() // 위젯이 처음 나타날 때 한번만 실행
{
	Super::NativeConstruct();

	if (SkillButton)
	{
		SkillButton->OnClicked.AddDynamic(this, &UCommonUserWidget_Skill_Slot::OnSkillButtonClicked);
	}


	//DataAsset slot
	if (this->GetName().Contains(TEXT("WBP_Skill_Slot_1")))
	{
		if (UGameInstance* GI = UGameplayStatics::GetGameInstance(GetWorld()))
		{
			if (UPro_M_GameInstance* ProGI = Cast<UPro_M_GameInstance>(GI))
			{
				if (ProGI->SkillDataAsset &&
					(ProGI->SkillDataAsset->PassiveSkills.Num() > 0 || ProGI->SkillDataAsset->ActiveSkills.Num() > 0))
				{
					APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
					USkillInventoryComponent* SkillInventory = PlayerCharacter ? PlayerCharacter->FindComponentByClass<USkillInventoryComponent>() : nullptr;

					//선택 가능한 스킬 목록
					TArray<FPassiveItemData> AvailablePassiveSkills;
					TArray<FActiveSkillItemData> AvailableActiveSkills;

					AvailablePassiveSkills.Empty();
					AvailableActiveSkills.Empty();

					// ---- Passive Filtering ----
					if (SkillInventory)
					{
						// --- Passive Filtering ---
						for (const FPassiveItemData& SkillData : ProGI->SkillDataAsset->PassiveSkills)
						{
							bool bAlreadyInInventory = false;
							bool bIsMaxStack = false;

							for (const FPassiveItemData& InvSkill : SkillInventory->PassiveSkillsInv)
							{
								if (InvSkill.Type == SkillData.Type)
								{
									bAlreadyInInventory = true;
									if (InvSkill.StackCnt >= InvSkill.MaxStackCnt)
									{
										bIsMaxStack = true;
									}
									break;
								}
							}

							// 슬롯 꽉 찼어도, 이미 인벤토리에 있고 최대 스택이 아니면 후보에 추가
							if (bAlreadyInInventory && !bIsMaxStack)
							{
								AvailablePassiveSkills.Add(SkillData);
							}
							// 슬롯 여유 있으면 새 스킬도 후보에 포함, 단 최대 스택 초과 제외
							else if (SkillInventory->PassiveSkillsInv.Num() < 4 && !bIsMaxStack)
							{
								AvailablePassiveSkills.Add(SkillData);
							}
						}

						// --- Active Filtering ---
						for (const FActiveSkillItemData& SkillData : ProGI->SkillDataAsset->ActiveSkills)
						{
							bool bAlreadyInInventory = false;
							bool bIsMaxStack = false;

							for (const FActiveSkillItemData& InvSkill : SkillInventory->ActiveSkillsInv)
							{
								if (InvSkill.Type == SkillData.Type)
								{
									bAlreadyInInventory = true;
									if (InvSkill.StackCnt >= InvSkill.MaxStackCnt)
									{
										bIsMaxStack = true;
									}
									break;
								}
							}

							if (bAlreadyInInventory && !bIsMaxStack)
							{
								AvailableActiveSkills.Add(SkillData);
							}
							else if (SkillInventory->ActiveSkillsInv.Num() < 4 && !bIsMaxStack)
							{
								AvailableActiveSkills.Add(SkillData);
							}
						}
					}

					//스킬이 전부 최대 스택일 경우
					if (AvailablePassiveSkills.Num() == 0 && AvailableActiveSkills.Num() == 0)
					{
						UE_LOG(LogTemp, Warning, TEXT("No available skills to show in slot 1 (All skills maxed)."));
						return;
					}

					// ---- Passive or Active 랜덤 선택 ----
					bool bChoosePassive = true;

					//둘 다 후보가 있으면 50% 확률로 선택
					if (AvailablePassiveSkills.Num() > 0 && AvailableActiveSkills.Num() > 0)
					{
						bChoosePassive = FMath::RandBool();
					}
					//Passive 후보가 없으면 Active 선택
					else if (AvailablePassiveSkills.Num() == 0 && AvailableActiveSkills.Num() > 0)
					{
						bChoosePassive = false;
					}
					//Active 후보가 없으면 Passive 선택
					else if (AvailableActiveSkills.Num() == 0 && AvailablePassiveSkills.Num() > 0)
					{
						bChoosePassive = true;
					}
					//둘 다 후보가 없으면 그냥 false (아무것도 선택 불가)
					else
					{
						bChoosePassive = false;
					}

					// ---- 실제 선택 및 UI 세팅 ----
					if (bChoosePassive)
					{
						int32 RandomIndex = FMath::RandRange(0, AvailablePassiveSkills.Num() - 1);
						const FPassiveItemData& Skill = AvailablePassiveSkills[RandomIndex];

						int32 DisplayStack = Skill.StackCnt;
						if (SkillInventory)
						{
							for (const FPassiveItemData& InvSkill : SkillInventory->PassiveSkillsInv)
							{
								if (InvSkill.Type == Skill.Type)
								{
									DisplayStack = FMath::Min(InvSkill.StackCnt + 1, InvSkill.MaxStackCnt);
									break;
								}
							}
						}

						SavedPassiveSkillData = Skill;
						bIsPassiveSlot = true;

						if (SkillNameText) SkillNameText->SetText(Skill.SkillName);
						if (Skill.Icon) SkillIcon->SetBrushFromTexture(Skill.Icon);
						if (SkillDescriptionText) SkillDescriptionText->SetText(Skill.Description);
						SelectedSkillImage = SkillIcon;
						if (StackText)
						{
							FString StackLevel = FString::Printf(TEXT("Lv.%d"), DisplayStack);
							StackText->SetText(FText::FromString(StackLevel));
						}
					}
					else
					{
						int32 RandomIndex = FMath::RandRange(0, AvailableActiveSkills.Num() - 1);
						const FActiveSkillItemData& Skill = AvailableActiveSkills[RandomIndex];

						int32 DisplayStack = Skill.StackCnt;
						if (SkillInventory)
						{
							for (const FActiveSkillItemData& InvSkill : SkillInventory->ActiveSkillsInv)
							{
								if (InvSkill.Type == Skill.Type)
								{
									DisplayStack = FMath::Min(InvSkill.StackCnt + 1, InvSkill.MaxStackCnt);
									break;
								}
							}
						}

						SavedActiveSkillData = Skill;
						bIsPassiveSlot = false;

						if (SkillNameText) SkillNameText->SetText(Skill.SkillName);
						if (Skill.Icon) SkillIcon->SetBrushFromTexture(Skill.Icon);
						if (SkillDescriptionText) SkillDescriptionText->SetText(Skill.Description);
						SelectedSkillImage = SkillIcon;
						if (StackText)
						{
							FString StackLevel = FString::Printf(TEXT("Lv.%d"), DisplayStack);
							StackText->SetText(FText::FromString(StackLevel));
						}
					}
				}
			}
		}
	}

	if (this->GetName().Contains(TEXT("WBP_Skill_Slot_2")))
	{
		if (UGameInstance* GI = UGameplayStatics::GetGameInstance(GetWorld()))
		{
			if (UPro_M_GameInstance* ProGI = Cast<UPro_M_GameInstance>(GI))
			{
				if (ProGI->SkillDataAsset &&
					(ProGI->SkillDataAsset->PassiveSkills.Num() > 0 || ProGI->SkillDataAsset->ActiveSkills.Num() > 0))
				{
					APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
					USkillInventoryComponent* SkillInventory = PlayerCharacter ? PlayerCharacter->FindComponentByClass<USkillInventoryComponent>() : nullptr;

					//선택 가능한 스킬 목록
					TArray<FPassiveItemData> AvailablePassiveSkills;
					TArray<FActiveSkillItemData> AvailableActiveSkills;
					AvailablePassiveSkills.Empty();
					AvailableActiveSkills.Empty();

					// ---- Passive Filtering ----
					if (SkillInventory)
					{
						for (const FPassiveItemData& SkillData : ProGI->SkillDataAsset->PassiveSkills)
						{
							bool bAlreadyInInventory = false;
							bool bIsMaxStack = false;

							for (const FPassiveItemData& InvSkill : SkillInventory->PassiveSkillsInv)
							{
								if (InvSkill.Type == SkillData.Type)
								{
									bAlreadyInInventory = true;
									if (InvSkill.StackCnt >= InvSkill.MaxStackCnt)
									{
										bIsMaxStack = true;
									}
									break;
								}
							}

							// 슬롯 꽉 찼어도, 이미 인벤토리에 있고 최대 스택이 아니면 후보에 추가
							if (bAlreadyInInventory && !bIsMaxStack)
							{
								AvailablePassiveSkills.Add(SkillData);
							}
							// 슬롯 여유 있으면 새 스킬도 후보에 포함, 단 최대 스택 초과 제외
							else if (SkillInventory->PassiveSkillsInv.Num() < 4 && !bIsMaxStack)
							{
								AvailablePassiveSkills.Add(SkillData);
							}
						}

						// --- Active Filtering ---
						for (const FActiveSkillItemData& SkillData : ProGI->SkillDataAsset->ActiveSkills)
						{
							bool bAlreadyInInventory = false;
							bool bIsMaxStack = false;

							for (const FActiveSkillItemData& InvSkill : SkillInventory->ActiveSkillsInv)
							{
								if (InvSkill.Type == SkillData.Type)
								{
									bAlreadyInInventory = true;
									if (InvSkill.StackCnt >= InvSkill.MaxStackCnt)
									{
										bIsMaxStack = true;
									}
									break;
								}
							}

							if (bAlreadyInInventory && !bIsMaxStack)
							{
								AvailableActiveSkills.Add(SkillData);
							}
							else if (SkillInventory->ActiveSkillsInv.Num() < 4 && !bIsMaxStack)
							{
								AvailableActiveSkills.Add(SkillData);
							}
						}
					}

					//스킬이 전부 최대 스택일 경우
					if (AvailablePassiveSkills.Num() == 0 && AvailableActiveSkills.Num() == 0)
					{
						UE_LOG(LogTemp, Warning, TEXT("No available skills to show in slot 1 (All skills maxed)."));
						return;
					}

					// ---- Passive or Active 랜덤 선택 ----
					bool bChoosePassive = true;

					// 둘 다 후보가 있으면 50% 확률로 선택
					if (AvailablePassiveSkills.Num() > 0 && AvailableActiveSkills.Num() > 0)
					{
						bChoosePassive = FMath::RandBool();
					}
					// Passive 후보가 없으면 Active 선택
					else if (AvailablePassiveSkills.Num() == 0 && AvailableActiveSkills.Num() > 0)
					{
						bChoosePassive = false;
					}
					// Active 후보가 없으면 Passive 선택
					else if (AvailableActiveSkills.Num() == 0 && AvailablePassiveSkills.Num() > 0)
					{
						bChoosePassive = true;
					}
					// 둘 다 후보가 없으면 그냥 false (아무것도 선택 불가)
					else
					{
						bChoosePassive = false;
					}

					// ---- 실제 선택 및 UI 세팅 ----
					if (bChoosePassive)
					{
						int32 RandomIndex = FMath::RandRange(0, AvailablePassiveSkills.Num() - 1);
						const FPassiveItemData& Skill = AvailablePassiveSkills[RandomIndex];

						int32 DisplayStack = Skill.StackCnt;
						if (SkillInventory)
						{
							for (const FPassiveItemData& InvSkill : SkillInventory->PassiveSkillsInv)
							{
								if (InvSkill.Type == Skill.Type)
								{
									DisplayStack = FMath::Min(InvSkill.StackCnt + 1, InvSkill.MaxStackCnt);
									break;
								}
							}
						}

						SavedPassiveSkillData = Skill;
						bIsPassiveSlot = true;

						if (SkillNameText) SkillNameText->SetText(Skill.SkillName);
						if (Skill.Icon) SkillIcon->SetBrushFromTexture(Skill.Icon);
						if (SkillDescriptionText) SkillDescriptionText->SetText(Skill.Description);
						SelectedSkillImage = SkillIcon;
						if (StackText)
						{
							FString StackLevel = FString::Printf(TEXT("Lv.%d"), DisplayStack);
							StackText->SetText(FText::FromString(StackLevel));
						}
					}
					else
					{
						int32 RandomIndex = FMath::RandRange(0, AvailableActiveSkills.Num() - 1);
						const FActiveSkillItemData& Skill = AvailableActiveSkills[RandomIndex];

						int32 DisplayStack = Skill.StackCnt;
						if (SkillInventory)
						{
							for (const FActiveSkillItemData& InvSkill : SkillInventory->ActiveSkillsInv)
							{
								if (InvSkill.Type == Skill.Type)
								{
									DisplayStack = FMath::Min(InvSkill.StackCnt + 1, InvSkill.MaxStackCnt);
									break;
								}
							}
						}

						SavedActiveSkillData = Skill;
						bIsPassiveSlot = false;

						if (SkillNameText) SkillNameText->SetText(Skill.SkillName);
						if (Skill.Icon) SkillIcon->SetBrushFromTexture(Skill.Icon);
						if (SkillDescriptionText) SkillDescriptionText->SetText(Skill.Description);
						SelectedSkillImage = SkillIcon;
						if (StackText)
						{
							FString StackLevel = FString::Printf(TEXT("Lv.%d"), DisplayStack);
							StackText->SetText(FText::FromString(StackLevel));
						}
					}
				}
			}
		}
	}
	if (this->GetName().Contains(TEXT("WBP_Skill_Slot_3")))
	{
		if (UGameInstance* GI = UGameplayStatics::GetGameInstance(GetWorld()))
		{
			if (UPro_M_GameInstance* ProGI = Cast<UPro_M_GameInstance>(GI))
			{
				if (ProGI->SkillDataAsset &&
					(ProGI->SkillDataAsset->PassiveSkills.Num() > 0 || ProGI->SkillDataAsset->ActiveSkills.Num() > 0))
				{
					APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
					USkillInventoryComponent* SkillInventory = PlayerCharacter ? PlayerCharacter->FindComponentByClass<USkillInventoryComponent>() : nullptr;

					//선택 가능한 스킬 목록
					TArray<FPassiveItemData> AvailablePassiveSkills;
					TArray<FActiveSkillItemData> AvailableActiveSkills;
					AvailablePassiveSkills.Empty();
					AvailableActiveSkills.Empty();

					// ---- Passive Filtering ----
					if (SkillInventory)
					{
						for (const FPassiveItemData& SkillData : ProGI->SkillDataAsset->PassiveSkills)
						{
							bool bAlreadyInInventory = false;
							bool bIsMaxStack = false;

							for (const FPassiveItemData& InvSkill : SkillInventory->PassiveSkillsInv)
							{
								if (InvSkill.Type == SkillData.Type)
								{
									bAlreadyInInventory = true;
									if (InvSkill.StackCnt >= InvSkill.MaxStackCnt)
									{
										bIsMaxStack = true;
									}
									break;
								}
							}

							// 슬롯 꽉 찼어도, 이미 인벤토리에 있고 최대 스택이 아니면 후보에 추가
							if (bAlreadyInInventory && !bIsMaxStack)
							{
								AvailablePassiveSkills.Add(SkillData);
							}
							// 슬롯 여유 있으면 새 스킬도 후보에 포함, 단 최대 스택 초과 제외
							else if (SkillInventory->PassiveSkillsInv.Num() < 4 && !bIsMaxStack)
							{
								AvailablePassiveSkills.Add(SkillData);
							}
						}

						// --- Active Filtering ---
						for (const FActiveSkillItemData& SkillData : ProGI->SkillDataAsset->ActiveSkills)
						{
							bool bAlreadyInInventory = false;
							bool bIsMaxStack = false;

							for (const FActiveSkillItemData& InvSkill : SkillInventory->ActiveSkillsInv)
							{
								if (InvSkill.Type == SkillData.Type)
								{
									bAlreadyInInventory = true;
									if (InvSkill.StackCnt >= InvSkill.MaxStackCnt)
									{
										bIsMaxStack = true;
									}
									break;
								}
							}

							if (bAlreadyInInventory && !bIsMaxStack)
							{
								AvailableActiveSkills.Add(SkillData);
							}
							else if (SkillInventory->ActiveSkillsInv.Num() < 4 && !bIsMaxStack)
							{
								AvailableActiveSkills.Add(SkillData);
							}
						}
					}

					//스킬이 전부 최대 스택일 경우
					if (AvailablePassiveSkills.Num() == 0 && AvailableActiveSkills.Num() == 0)
					{
						UE_LOG(LogTemp, Warning, TEXT("No available skills to show in slot 1 (All skills maxed)."));
						return;
					}

					// ---- Passive or Active 랜덤 선택 ----
					bool bChoosePassive = true;

					// 둘 다 후보가 있으면 50% 확률로 선택
					if (AvailablePassiveSkills.Num() > 0 && AvailableActiveSkills.Num() > 0)
					{
						bChoosePassive = FMath::RandBool();
					}
					// Passive 후보가 없으면 Active 선택
					else if (AvailablePassiveSkills.Num() == 0 && AvailableActiveSkills.Num() > 0)
					{
						bChoosePassive = false;
					}
					// Active 후보가 없으면 Passive 선택
					else if (AvailableActiveSkills.Num() == 0 && AvailablePassiveSkills.Num() > 0)
					{
						bChoosePassive = true;
					}
					// 둘 다 후보가 없으면 그냥 false (아무것도 선택 불가)
					if (AvailablePassiveSkills.Num() == 0 && AvailableActiveSkills.Num() == 0)
					{
						bChoosePassive = false;
						return;
					}

					// ---- 실제 선택 및 UI 세팅 ----
					if (bChoosePassive)
					{
						int32 RandomIndex = FMath::RandRange(0, AvailablePassiveSkills.Num() - 1);
						const FPassiveItemData& Skill = AvailablePassiveSkills[RandomIndex];

						int32 DisplayStack = Skill.StackCnt;
						if (SkillInventory)
						{
							for (const FPassiveItemData& InvSkill : SkillInventory->PassiveSkillsInv)
							{
								if (InvSkill.Type == Skill.Type)
								{
									DisplayStack = FMath::Min(InvSkill.StackCnt + 1, InvSkill.MaxStackCnt);
									break;
								}
							}
						}

						SavedPassiveSkillData = Skill;
						bIsPassiveSlot = true;

						if (SkillNameText) SkillNameText->SetText(Skill.SkillName);
						if (Skill.Icon) SkillIcon->SetBrushFromTexture(Skill.Icon);
						if (SkillDescriptionText) SkillDescriptionText->SetText(Skill.Description);
						SelectedSkillImage = SkillIcon;
						if (StackText)
						{
							FString StackLevel = FString::Printf(TEXT("Lv.%d"), DisplayStack);
							StackText->SetText(FText::FromString(StackLevel));
						}
					}
					else
					{
						int32 RandomIndex = FMath::RandRange(0, AvailableActiveSkills.Num() - 1);
						const FActiveSkillItemData& Skill = AvailableActiveSkills[RandomIndex];

						int32 DisplayStack = Skill.StackCnt;
						if (SkillInventory)
						{
							for (const FActiveSkillItemData& InvSkill : SkillInventory->ActiveSkillsInv)
							{
								if (InvSkill.Type == Skill.Type)
								{
									DisplayStack = FMath::Min(InvSkill.StackCnt + 1, InvSkill.MaxStackCnt);
									break;
								}
							}
						}

						SavedActiveSkillData = Skill;
						bIsPassiveSlot = false;

						if (SkillNameText) SkillNameText->SetText(Skill.SkillName);
						if (Skill.Icon) SkillIcon->SetBrushFromTexture(Skill.Icon);
						if (SkillDescriptionText) SkillDescriptionText->SetText(Skill.Description);
						SelectedSkillImage = SkillIcon;
						if (StackText)
						{
							FString StackLevel = FString::Printf(TEXT("Lv.%d"), DisplayStack);
							StackText->SetText(FText::FromString(StackLevel));
						}
					}
				}
			}
		}
	}
}

void UCommonUserWidget_Skill_Slot::OnSkillButtonClicked()
{
	if (!SelectedSkillImage)
	{
		UE_LOG(LogTemp, Error, TEXT("SelectedSkillImage is NULL. Button logic failed to start."));
		return;
	}

	//플레이어 및 컴포넌트 참조 가져오기
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!PlayerCharacter) {
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter not found."));
		return;
	}

	USkillInventoryComponent* SkillInventory = PlayerCharacter->FindComponentByClass<USkillInventoryComponent>();
	UCharacterStatsComponent* StatComponent = PlayerCharacter->FindComponentByClass<UCharacterStatsComponent>();

	if (!SkillInventory || !StatComponent) {
		UE_LOG(LogTemp, Error, TEXT("SkillInventory or StatComponent not found."));
		return;
	}

	//StatComponent의 초기 공격력을 저장
	if (FMath::IsNearlyZero(StaticBaseAttackDamage))
	{
		StaticBaseAttackDamage = StatComponent->AttackDamage;
		UE_LOG(LogTemp, Log, TEXT("STATIC Base Attack CACHED: %.1f"), StaticBaseAttackDamage);
	}

	//최종적으로 HUD에 표시할 스택 변수
	int32 FinalDisplayStack = 0;
	int32 MaxStackToApply = 0;

	// ==============================================================================
	// Passive Skill 처리
	// ==============================================================================
	if (bIsPassiveSlot)
{
    // 1. 기존 Passive 스킬 존재 여부 확인 및 포인터 가져오기
    int32 CurrentStack = 0;
    FPassiveItemData* ExistingSkill = nullptr;

    for (FPassiveItemData& Skill : SkillInventory->PassiveSkillsInv)
    {
        if (Skill.Type == SavedPassiveSkillData.Type)
        {
            ExistingSkill = &Skill;
            break;
        }
    }

    // 2. 스택 증가 또는 새 스킬 추가 로직 실행
    if (ExistingSkill)
    {
        if (ExistingSkill->StackCnt < ExistingSkill->MaxStackCnt)
        {
            ExistingSkill->StackCnt++;
            CurrentStack = ExistingSkill->StackCnt;
        }
        else
        {
            CurrentStack = ExistingSkill->StackCnt;
        }
    }
    else
    {
        //4개 제한 체크
        if (SkillInventory->PassiveSkillsInv.Num() >= 4)
        {
            UE_LOG(LogTemp, Warning, TEXT("[SkillSlot] Passive skill limit reached (4/4). Cannot add %s."),
                *SavedPassiveSkillData.SkillName.ToString());
            return;
        }

        //Passive 새 스킬 추가
        FPassiveItemData NewSkill = SavedPassiveSkillData;
        NewSkill.StackCnt = 1;

        SkillInventory->PassiveSkillsInv.Add(NewSkill);
        CurrentStack = NewSkill.StackCnt;

        UE_LOG(LogTemp, Log, TEXT("Passive Skill inv Added! Name: %s, Stack: %d. Size: %d"),
            *NewSkill.SkillName.ToString(), CurrentStack, SkillInventory->PassiveSkillsInv.Num());
    }

    //스택 정보 업데이트
    MaxStackToApply = ExistingSkill ? ExistingSkill->MaxStackCnt : SavedPassiveSkillData.MaxStackCnt;
    FinalDisplayStack = FMath::Min(CurrentStack, MaxStackToApply);
}

	// ==============================================================================
	// Active Skill 처리
	// ==============================================================================
	else // bIsPassiveSlot == false
	{
		//기존 Active 스킬 존재 여부 확인 및 포인터 가져오기
		int32 CurrentStack = 0;
		FActiveSkillItemData* ExistingActiveSkill = nullptr;

		for (FActiveSkillItemData& Skill : SkillInventory->ActiveSkillsInv)
		{
			if (Skill.Type == SavedActiveSkillData.Type)
			{
				ExistingActiveSkill = &Skill;
				break;
			}
		}

		//스택 증가 또는 새 스킬 추가 로직 실행
		if (ExistingActiveSkill)
		{
			if (ExistingActiveSkill->StackCnt < ExistingActiveSkill->MaxStackCnt)
			{
				ExistingActiveSkill->StackCnt++;
				CurrentStack = ExistingActiveSkill->StackCnt;
			}
			else
			{
				CurrentStack = ExistingActiveSkill->StackCnt;
			}
		}
		else
		{
			//Active 새 스킬 추가
			FActiveSkillItemData NewActiveSkill = SavedActiveSkillData;
			NewActiveSkill.StackCnt = 1;

			SkillInventory->ActiveSkillsInv.Add(NewActiveSkill);
			CurrentStack = NewActiveSkill.StackCnt;

			UE_LOG(LogTemp, Log, TEXT("Active Skill inv Added! Name: %s, Stack: %d. Size: %d"),
				*NewActiveSkill.SkillName.ToString(), CurrentStack, SkillInventory->ActiveSkillsInv.Num());
		}

		//스택 정보 업데이트
		MaxStackToApply = ExistingActiveSkill ? ExistingActiveSkill->MaxStackCnt : SavedActiveSkillData.MaxStackCnt;
		FinalDisplayStack = FMath::Min(CurrentStack, MaxStackToApply);
	}
	if (UWorld* World = GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = PlayerCharacter;

		//ApassiveItem 스폰 (별도의 블루프린트 지정 없이 C++ 기본 클래스 사용)
		ApassiveItem* NewPassive = World->SpawnActor<ApassiveItem>(
			ApassiveItem::StaticClass(),
			PlayerCharacter->GetActorLocation(),
			FRotator::ZeroRotator,
			SpawnParams
		);

		if (NewPassive)
		{
			//타입 및 데이터 전달
			NewPassive->PassiveType = SavedPassiveSkillData.Type;
			NewPassive->PassiveSkillData = SavedPassiveSkillData;

			//효과 적용
			NewPassive->PassiveSkillApply(PlayerCharacter);
		}
	}

	//HUD 갱신
	if (UCommonUserWidget_BattleGameHUD* BattleHUD = GetBattleHUD())
	{
		if (!SelectedSkillImage) return;
		int32 DisplayLevel = FMath::Min(FinalDisplayStack, 5);

		// Passive Skill
		if (bIsPassiveSlot)
		{
			//1번 슬롯 (기존 스킬이거나 빈 슬롯인 경우 갱신)
			if (BattleHUD->SkillSlot_1_Type == SavedPassiveSkillData.Type || BattleHUD->SkillSlot_1_Type == EPassiveItemType::None)
			{
				BattleHUD->Image_Skill_1->SetBrush(SelectedSkillImage->GetBrush());
				BattleHUD->Image_Skill_1->SetColorAndOpacity(FLinearColor::White);
				BattleHUD->PassiveStackCnt_1->SetText(FText::FromString(FString::Printf(TEXT("Lv.%d"), DisplayLevel)));
				BattleHUD->PassiveStackCnt_1->SetVisibility(ESlateVisibility::Visible);
				BattleHUD->SkillSlot_1_Type = SavedPassiveSkillData.Type; // Passive Type 저장
			}
			else if (BattleHUD->SkillSlot_2_Type == SavedPassiveSkillData.Type || BattleHUD->SkillSlot_2_Type == EPassiveItemType::None)
			{
				BattleHUD->Image_Skill_2->SetBrush(SelectedSkillImage->GetBrush());
				BattleHUD->Image_Skill_2->SetColorAndOpacity(FLinearColor::White);
				BattleHUD->PassiveStackCnt_2->SetText(FText::FromString(FString::Printf(TEXT("Lv.%d"), DisplayLevel)));
				BattleHUD->PassiveStackCnt_2->SetVisibility(ESlateVisibility::Visible);
				BattleHUD->SkillSlot_2_Type = SavedPassiveSkillData.Type;
			}
			else if (BattleHUD->SkillSlot_3_Type == SavedPassiveSkillData.Type || BattleHUD->SkillSlot_3_Type == EPassiveItemType::None)
			{
				BattleHUD->Image_Skill_3->SetBrush(SelectedSkillImage->GetBrush());
				BattleHUD->Image_Skill_3->SetColorAndOpacity(FLinearColor::White);
				BattleHUD->PassiveStackCnt_3->SetText(FText::FromString(FString::Printf(TEXT("Lv.%d"), DisplayLevel)));
				BattleHUD->PassiveStackCnt_3->SetVisibility(ESlateVisibility::Visible);
				BattleHUD->SkillSlot_3_Type = SavedPassiveSkillData.Type;
			}
			else if (BattleHUD->SkillSlot_4_Type == SavedPassiveSkillData.Type || BattleHUD->SkillSlot_4_Type == EPassiveItemType::None)
			{
				BattleHUD->Image_Skill_4->SetBrush(SelectedSkillImage->GetBrush());
				BattleHUD->Image_Skill_4->SetColorAndOpacity(FLinearColor::White);
				BattleHUD->PassiveStackCnt_4->SetText(FText::FromString(FString::Printf(TEXT("Lv.%d"), DisplayLevel)));
				BattleHUD->PassiveStackCnt_4->SetVisibility(ESlateVisibility::Visible);
				BattleHUD->SkillSlot_4_Type = SavedPassiveSkillData.Type;
			}
		}
		//Active Skill HUD 갱신
		else
		{
			// 1번 슬롯
			if (BattleHUD->SkillSlot_1_Active == SavedActiveSkillData.Type || BattleHUD->SkillSlot_1_Active == EActiveSkillItemType::None)
			{
				if (BattleHUD->Image_Skill_5 && BattleHUD->ActiveStackCnt_1)
				{
					BattleHUD->Image_Skill_5->SetBrush(SelectedSkillImage->GetBrush());
					BattleHUD->Image_Skill_5->SetColorAndOpacity(FLinearColor::White);
					BattleHUD->ActiveStackCnt_1->SetText(FText::FromString(FString::Printf(TEXT("Lv.%d"), DisplayLevel)));
					BattleHUD->ActiveStackCnt_1->SetVisibility(ESlateVisibility::Visible);
				}
				BattleHUD->SkillSlot_1_Active = SavedActiveSkillData.Type;
			}
			else if (BattleHUD->SkillSlot_2_Active == SavedActiveSkillData.Type || BattleHUD->SkillSlot_2_Active == EActiveSkillItemType::None)
			{
				if (BattleHUD->Image_Skill_6 && BattleHUD->ActiveStackCnt_2)
				{
					BattleHUD->Image_Skill_6->SetBrush(SelectedSkillImage->GetBrush());
					BattleHUD->Image_Skill_6->SetColorAndOpacity(FLinearColor::White);
					BattleHUD->ActiveStackCnt_2->SetText(FText::FromString(FString::Printf(TEXT("Lv.%d"), DisplayLevel)));
					BattleHUD->ActiveStackCnt_2->SetVisibility(ESlateVisibility::Visible);
				}
				BattleHUD->SkillSlot_2_Active = SavedActiveSkillData.Type;
			}
			else if (BattleHUD->SkillSlot_3_Active == SavedActiveSkillData.Type || BattleHUD->SkillSlot_3_Active == EActiveSkillItemType::None)
			{
				if (BattleHUD->Image_Skill_7 && BattleHUD->ActiveStackCnt_3)
				{
					BattleHUD->Image_Skill_7->SetBrush(SelectedSkillImage->GetBrush());
					BattleHUD->Image_Skill_7->SetColorAndOpacity(FLinearColor::White);
					BattleHUD->ActiveStackCnt_3->SetText(FText::FromString(FString::Printf(TEXT("Lv.%d"), DisplayLevel)));
					BattleHUD->ActiveStackCnt_3->SetVisibility(ESlateVisibility::Visible);
				}
				BattleHUD->SkillSlot_3_Active = SavedActiveSkillData.Type;
			}
			else if (BattleHUD->SkillSlot_4_Active == SavedActiveSkillData.Type || BattleHUD->SkillSlot_4_Active == EActiveSkillItemType::None)
			{
				if (BattleHUD->Image_Skill_8 && BattleHUD->ActiveStackCnt_4)
				{
					BattleHUD->Image_Skill_8->SetBrush(SelectedSkillImage->GetBrush());
					BattleHUD->Image_Skill_8->SetColorAndOpacity(FLinearColor::White);
					BattleHUD->ActiveStackCnt_4->SetText(FText::FromString(FString::Printf(TEXT("Lv.%d"), DisplayLevel)));
					BattleHUD->ActiveStackCnt_4->SetVisibility(ESlateVisibility::Visible);
				}
				BattleHUD->SkillSlot_4_Active = SavedActiveSkillData.Type;
			}
		}

		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			PC->SetPause(false);
			PC->SetShowMouseCursor(true);

			//입력 모드 복구 게임 전용
			FInputModeGameOnly InputMode;
			PC->SetInputMode(InputMode);
		}

		//뷰포트 갱신
		if (UWorld* World = GetWorld())
		{
			TArray<UUserWidget*> FoundSkillWidgets;

			//월드에 존재하는 UCommonUserWidget_Skill 클래스의 모든 인스턴스를 검색합니다.
			UWidgetBlueprintLibrary::GetAllWidgetsOfClass(World, FoundSkillWidgets, UCommonUserWidget_Skill::StaticClass(), false);

			for (UUserWidget* SkillWidget : FoundSkillWidgets)
			{
				//찾은 스킬 위젯을 제거합니다. (스킬 창 전체가 닫힘)
				if (SkillWidget)
				{
					SkillWidget->RemoveFromParent();
				}
			}
		}
	}
}

UCommonUserWidget_BattleGameHUD* UCommonUserWidget_Skill_Slot::GetBattleHUD()
{
	if (UWorld* World = GetWorld())
	{
		TArray<UUserWidget*> FoundWidgets;

		//현재 월드에 존재하는 BattleGameHUD 클래스의 위젯을 검색
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(World, FoundWidgets, UCommonUserWidget_BattleGameHUD::StaticClass(), false);

		if (FoundWidgets.Num() > 0)
		{
			return Cast<UCommonUserWidget_BattleGameHUD>(FoundWidgets[0]);
		}
	}
	return nullptr;
}


