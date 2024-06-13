// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientScreen.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
void UClientScreen::NativeConstruct()
{
	Super::NativeConstruct();
	ConnectButton->OnClicked.AddDynamic(this, &UClientScreen::ConnectButtonClick);
}

void UClientScreen::SetPlayerCount(int32 PlayerCount)
{
	FText Player = FText::FromString(FString::FromInt(PlayerCount));
	PlayerNum->SetText(Player);
}

void UClientScreen::ConnectButtonClick()
{
	FText MyText = Ip_Text->GetText();
	if (MyText.IsEmpty())
	{
		return;
	}
	//connect logic

	SuccessText->SetText(FText::FromString("성공")); // 성공 시
	//SuccessText->SetText(FText::FromString("실패")); 실패 시
}
