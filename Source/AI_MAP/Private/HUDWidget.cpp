// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "CharacterIconWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Engine/Texture2D.h"
#include "SkillWidget.h"
#include "Components/TextBlock.h"

#include "SkillChooseWidget.h"

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	CharacterWidget = Cast<UCharacterIconWidget>(GetWidgetFromName(TEXT("WBP_CharacterIcon")));

	ChooseSkillWidget = Cast<USkillChooseWidget>(GetWidgetFromName(TEXT("WBP_SkillChoose")));
	CurrentGuardText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentGuard")));
	MaxGuardText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaxGuard")));
	PanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ChooseSkillWidget);
}
void UHUDWidget::ShowChooseSkill()
{
	FVector2D pos(0, -600);
	if (PanelSlot == nullptr)
	{
		return;
	}
	PanelSlot->SetPosition(pos);
}

void UHUDWidget::HideChooseSkill()
{
	FVector2D pos(0, 0);
	if (PanelSlot == nullptr)
	{
		return;
	}
	PanelSlot->SetPosition(pos);
}

void UHUDWidget::SetChooseSkillImage(TArray<class UTexture2D*>& RandomTextureArray)
{
	if (ChooseSkillWidget == nullptr)
	{
		return;
	}
	ChooseSkillWidget->SetSelectSkillImage(RandomTextureArray);
}

void UHUDWidget::UpdateSkillLevel(int32 Index, int32 Level)
{
	if (CharacterWidget == nullptr)
	{
		return;
	}
	CharacterWidget->UpdateSkillLevel(Index,Level);
}

void UHUDWidget::UpdateSkillImage(UTexture2D* Texture ,int32 Index)
{
	if (CharacterWidget == nullptr)
	{
		return;
	}
	CharacterWidget->UpdateSkillImage(Texture,Index);
}

void UHUDWidget::UpdateHpBar(int32 MaxHP, int32 CurrHp)
{
	if (CharacterWidget == nullptr)
	{
		return;
	}
	CharacterWidget->UpdateHpBar(MaxHP, CurrHp);
}

void UHUDWidget::UpdateExpBar(float MaxExp, float CurrExp)
{
	if (CharacterWidget == nullptr)
	{
		return;
	}
	CharacterWidget->UpdateExpBar(MaxExp, CurrExp);
}

void UHUDWidget::UpdateGuardPoint(int32 Cur, int32 Max)
{
	FText MaxText = FText::FromString(FString::FromInt(Max));
	FText CurText = FText::FromString(FString::FromInt(Cur));
	MaxGuardText->SetText(MaxText);
	CurrentGuardText->SetText(CurText);
}

void UHUDWidget::SetCharacterLevel(int32 Level)
{
	CharacterWidget->SetCharacterLevel(Level);
}
