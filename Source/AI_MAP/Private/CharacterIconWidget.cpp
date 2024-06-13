// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterIconWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"
#include "StatusBar.h"
#include "SkillWidget.h"


void UCharacterIconWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	StatBar = Cast<UStatusBar>(GetWidgetFromName(TEXT("WBP_StatusBar")));

	Skill = Cast<USkillWidget>(GetWidgetFromName(TEXT("WBP_Skill")));

	CharIconImage = Cast<UImage>(GetWidgetFromName(TEXT("CharacterImg")));
	
	CharLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("Level")));

	ExpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("EXP_Bar")));
}
USkillWidget* UCharacterIconWidget::GetSkillWidget()
{
	return Skill;
}

void UCharacterIconWidget::UpdateSkillImage(UTexture2D* Texture,int32 index)
{
	Skill->UpdateSkillImage(Texture, index);
}

void UCharacterIconWidget::UpdateSkillLevel(int32 index,int32 level)
{
	Skill->UpdateSkillLevel(index,level);
}

void UCharacterIconWidget::UpdateHpBar(int32 MaxHp, int32 CurrHp)
{
	StatBar->UpdateHealthBar(MaxHp, CurrHp);
}

void UCharacterIconWidget::UpdateExpBar(int32 MaxExp, int32 CurrExp)
{
	float Exp = float(CurrExp) / (float)MaxExp;
	ExpBar->SetPercent(Exp);
}

void UCharacterIconWidget::SetCharacterLevel(int32 MyLevel)
{
	FText Level = FText::FromString(FString::FromInt(MyLevel));
	CharLevel->SetText(Level);
}
