// Fill out your copyright notice in the Description page of Project Settings.


#include "ResultWidget.h"
#include "Components/TextBlock.h"
void UResultWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UResultWidget::SetResultText(bool bWin)
{
	if (bWin == true)
	{
		FText Result = FText::FromString(FString("Victory"));
		ResultText->SetText(Result);
	}
	else if (bWin == false)
	{
		FText Result = FText::FromString(FString("Defeat"));
		ResultText->SetText(Result);
		ResultText->SetColorAndOpacity(FLinearColor::Red);
	}
}
