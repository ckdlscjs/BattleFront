// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"
#include "Components/ProgressBar.h"


void UHealthBar::NativeConstruct()
{
	Super::NativeConstruct();
	//if (HealthBar == nullptr)
	//{
	//	HealthProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HealthBar")));
	//}
	//GuardProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("GuardBar")));
}
void UHealthBar::SetPercentage(float MaxHp, float CurrHp)
{
	float temp = (float)CurrHp / (float)MaxHp;
	//if (HealthProgressBar == nullptr)
	//{
	//	HealthProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HealthBar")));
	//}
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
