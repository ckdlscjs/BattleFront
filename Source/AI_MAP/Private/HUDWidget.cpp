// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "CharacterIconWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Engine/Texture2D.h"
#include "SkillWidget.h"
#include "SkillChooseWidget.h"

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	CharacterWidget = Cast<UCharacterIconWidget>(GetWidgetFromName(TEXT("WBP_CharacterIcon")));

	ChooseSkillWidget = Cast<USkillChooseWidget>(GetWidgetFromName(TEXT("WBP_SkillChoose")));

	PanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ChooseSkillWidget);
}
void UHUDWidget::ShowChooseSkill()
{
	FVector2D pos(0, -600);
	PanelSlot->SetPosition(pos);
}

void UHUDWidget::HideChooseSkill()
{
	FVector2D pos(0, 0);
	PanelSlot->SetPosition(pos);
}

void UHUDWidget::SetChooseSkillImage(TArray<class UTexture2D*>& RandomTextureArray)
{
	ChooseSkillWidget->SetSelectSkillImage(RandomTextureArray);
}

void UHUDWidget::UpdateSkillLevel(int32 Index, int32 Level)
{
	CharacterWidget->UpdateSkillLevel(Index,Level);
}

void UHUDWidget::UpdateSkillImage(UTexture2D* Texture ,int32 Index)
{
	CharacterWidget->UpdateSkillImage(Texture,Index);
}

void UHUDWidget::UpdateHpBar(int32 MaxHP, int32 CurrHp)
{
	CharacterWidget->UpdateHpBar(MaxHP, CurrHp);
}

void UHUDWidget::UpdateExpBar(float MaxExp, float CurrExp)
{
	CharacterWidget->UpdateExpBar(MaxExp, CurrExp);
}
