// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientScreen.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "NetworkManager.h"

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
		return;
	//connect logic
	if (GetNetworkManager()->ConnectToGameServer(MyText.ToString()))
		SuccessText->SetText(FText::FromString("success")); // ���� ��
	else
		SuccessText->SetText(FText::FromString("fail"));
}

UNetworkManager* UClientScreen::GetNetworkManager() const
{
	return GetGameInstance()->GetSubsystem<UNetworkManager>();
}
