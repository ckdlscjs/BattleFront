// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillLevelWidget.h"
#include "Components/StackBox.h"
#include "Components/CheckBox.h"

void USkillLevelWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	LevelBox = Cast<UStackBox>(GetWidgetFromName(TEXT("StackBox_24")));

	LevelArray.SetNum(5);

	Level1_Check = Cast<UCheckBox>(GetWidgetFromName(TEXT("Level1")));
	LevelArray[0] = Level1_Check;

	Level2_Check = Cast<UCheckBox>(GetWidgetFromName(TEXT("Level2")));
	LevelArray[1] = Level2_Check;

	Level3_Check = Cast<UCheckBox>(GetWidgetFromName(TEXT("Level3")));
	LevelArray[2] = Level3_Check;

	Level4_Check = Cast<UCheckBox>(GetWidgetFromName(TEXT("Level4")));
	LevelArray[3] = Level4_Check;

	Level5_Check = Cast<UCheckBox>(GetWidgetFromName(TEXT("Level5")));
	LevelArray[4] = Level5_Check;
}
void USkillLevelWidget::SetSkillLevel(int32 level)
{
	LevelArray[level]->SetCheckedState(ECheckBoxState::Checked);
}
