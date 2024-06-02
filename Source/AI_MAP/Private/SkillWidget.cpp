// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillWidget.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "SkillLevelWidget.h"
#include "Engine/Texture2D.h"

void USkillWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SkillImageSet = Cast<UHorizontalBox>(GetWidgetFromName(TEXT("SkillImageBox")));
	SkillLevelSet = Cast<UHorizontalBox>(GetWidgetFromName(TEXT("SkillLevelBox")));

	Q_SkillIamge = Cast<UImage>(GetWidgetFromName(TEXT("Q_Image")));
	Q_SkillWidget = Cast<USkillLevelWidget>(GetWidgetFromName(TEXT("QSkillLevel")));

	SkillArr.Add(Q_SkillIamge, Q_SkillWidget);
	SkillImageArr.Add(0, Q_SkillIamge);

	W_SkillIamge = Cast<UImage>(GetWidgetFromName(TEXT("W_Image")));
	W_SkillWidget = Cast<USkillLevelWidget>(GetWidgetFromName(TEXT("WSkillLevel")));
	SkillArr.Add(W_SkillIamge, W_SkillWidget);
	SkillImageArr.Add(1, W_SkillIamge);

	E_SkillIamge = Cast<UImage>(GetWidgetFromName(TEXT("E_Image")));
	E_SkillWidget = Cast<USkillLevelWidget>(GetWidgetFromName(TEXT("ESkillLevel")));
	SkillArr.Add(E_SkillIamge, E_SkillWidget);
	SkillImageArr.Add(2, E_SkillIamge);

	R_SkillIamge = Cast<UImage>(GetWidgetFromName(TEXT("R_Image")));
	R_SkillWidget = Cast<USkillLevelWidget>(GetWidgetFromName(TEXT("RSkillLevel")));
	SkillArr.Add(R_SkillIamge, R_SkillWidget);
	SkillImageArr.Add(3, R_SkillIamge);

	for (auto Data : SkillImageArr)
	{
		UImage* Image = Data.Value;
		Image->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USkillWidget::UpdateSkillImage(UTexture2D* Texture, int32 index)
{
	if (!SkillImageArr.Contains(index))
	{
		return;
	}
	UImage* image = SkillImageArr.FindRef(index);
	image->SetVisibility(ESlateVisibility::Visible);
	
	image->SetBrushFromTexture(Texture);
	
}

void USkillWidget::UpdateSkillLevel(int32 index,int32 level)
{
	UImage* image = SkillImageArr.FindRef(index);
	USkillLevelWidget* SLW = SkillArr.FindRef(image);
	SLW->SetSkillLevel(level);
}
