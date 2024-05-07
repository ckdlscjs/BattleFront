// Fill out your copyright notice in the Description page of Project Settings.
#include "StatusBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UStatusBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	HealthBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HP_Bar")));

	CurrentHealthText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentHealth")));

	MaxHealthText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaxHealth")));
	
}

void UStatusBar::UpdateHealthBar(int32 MaxHp, int32 CurrHp)
{
	FText MaxHealth = FText::FromString(FString::FromInt(MaxHp));
	FText CurHealth = FText::FromString(FString::FromInt(CurrHp));
	MaxHealthText->SetText(MaxHealth);
	CurrentHealthText->SetText(CurHealth);
	float temp = (float)CurrHp / (float)MaxHp;
	HealthBar->SetPercent(temp);
}