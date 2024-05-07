// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillChooseWidget.h"
#include "Components/Image.h"

void USkillChooseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	FirSkill = Cast<UImage>(GetWidgetFromName(TEXT("1stSkill")));
	ImageArray.Add(FirSkill);

	SecSkill = Cast<UImage>(GetWidgetFromName(TEXT("2ndSkill")));
	ImageArray.Add(SecSkill);

	ThirSkill = Cast<UImage>(GetWidgetFromName(TEXT("3rdSkill")));
	ImageArray.Add(ThirSkill);

	for (int32 i = 0; i < ImageArray.Num(); i++)
	{
		ImageArray[i]->SetOpacity(0.0f);
	}
	
}
void USkillChooseWidget::SetSelectSkillImage(TArray<class UTexture2D*>& TextureArray)
{
	int32 count = 0;
	for (int32 i = 0; i < TextureArray.Num(); i++)
	{
		ImageArray[i]->SetOpacity(1.0f);
		ImageArray[i]->SetBrushFromTexture(TextureArray[i]);
		count++;
	}
	for (int32 i = count; i < ImageArray.Num(); i++)
	{
		ImageArray[i]->SetOpacity(0.0f);
	}
}