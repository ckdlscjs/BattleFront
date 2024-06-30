// Fill out your copyright notice in the Description page of Project Settings.


#include "IPWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/CheckBox.h"
#include "Components/Button.h"
#include "NetworkManager.h"

void UIPWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InputButton->OnClicked.AddDynamic(this, &UIPWidget::InputButtonClick);
	if (HostCheck != nullptr)
	{
		HostCheck->OnCheckStateChanged.AddDynamic(this, &UIPWidget::HostCheckBoxClicked);
	}
	if (ClientCheck != nullptr)
	{
		ClientCheck->OnCheckStateChanged.AddDynamic(this, &UIPWidget::ClientCheckBoxClicked);
	}
	bIsHost = false;
}

void UIPWidget::InputButtonClick()
{
	if (bIsHost == false)
	{
		FText MyText = IP_Text->GetText();
		if (MyText.IsEmpty())
			return;
		//GetNetworkManager()->ConnectToGameServer(MyText.ToString());
	}
	else // Host
	{
		//FString findMyIP = GetNetworkManager()->GetLocalIPAddress();
		//GetNetworkManager()->ConnectToGameServer(findMyIP);
	}
}

void UIPWidget::HostCheckBoxClicked(bool bIsCheck)
{
	if (bIsCheck == true)
	{
		if (ClientCheck->IsChecked() == true)
		{
			ClientCheck->CheckedState = ECheckBoxState::Unchecked;
		}
		IP_Text->SetIsEnabled(true);
		bIsHost = true;
	}
}

void UIPWidget::ClientCheckBoxClicked(bool bIsCheck)
{
	if (bIsCheck == true)
	{
		if (HostCheck->IsChecked() == true)
		{
			HostCheck->CheckedState = ECheckBoxState::Unchecked;
		}
		IP_Text->SetIsEnabled(false);
		bIsHost = false;
	}
}

UNetworkManager* UIPWidget::GetNetworkManager() const
{
	return GetGameInstance()->GetSubsystem<UNetworkManager>();
}
