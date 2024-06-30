// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHealthBar::NativeConstruct()
{
	Super::NativeConstruct();

}
void UHealthBar::SetPercentage(float MaxHp, float CurrHp)
{
	float temp = (float)CurrHp / (float)MaxHp;
	HealthBar->SetPercent(temp);
}

void UHealthBar::SetGuardPercent(float MaxGuard, float CurrGurad)
{
	float temp = CurrGurad / MaxGuard;
	GuardBar->SetPercent(temp);
}

void UHealthBar::SetColorHpBar()
{
	HealthBar->SetFillColorAndOpacity(FLinearColor(0, 1, 0, 0.7));
}

void UHealthBar::SetLevelText(int32 Level)
{
	FText MyLevel = FText::FromString(FString::FromInt(Level));

	LevelText->SetText(MyLevel);
}
