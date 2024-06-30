// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "CharacterIconWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Engine/Texture2D.h"
#include "SkillWidget.h"
#include "Components/TextBlock.h"
#include "Team_AIMagneticField.h"
#include "Team_AIGameMode.h"
#include "SkillChooseWidget.h"
#include "Kismet/GameplayStatics.h"
#include "WorldMapWidget.h"

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	CharacterWidget = Cast<UCharacterIconWidget>(GetWidgetFromName(TEXT("WBP_CharacterIcon")));

	ChooseSkillWidget = Cast<USkillChooseWidget>(GetWidgetFromName(TEXT("WBP_SkillChoose")));
	CurrentGuardText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentGuard")));
	MaxGuardText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaxGuard")));
	WorldMapWidget = Cast<UWorldMapWidget>(GetWidgetFromName(TEXT("WBP_WorldMap")));
	PanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ChooseSkillWidget);
	WorldMapVisible = false;
	WorldMapWidget->SetVisibility(ESlateVisibility::Hidden);
	MyTimer = 0.f;
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

void UHUDWidget::SetMageticeFieldTimer(float Timer)
{
	FText Time = FText::FromString(FString::FromInt(Timer));
	TimerText->SetText(Time);
}

void UHUDWidget::ToggleWorldMap()
{
	WorldMapVisible = !WorldMapVisible;
	WorldMapWidget->SetVisibility(WorldMapVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (MagneticField == nullptr)
	{
		ATeam_AIGameMode* GameMode = Cast<ATeam_AIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		MagneticField = GameMode->GetMagneticField();
	}
	if (MagneticField != nullptr)
	{
		MyTimer = MagneticField->GetRemainTime();
		SetMageticeFieldTimer(MyTimer);
	}	
}
