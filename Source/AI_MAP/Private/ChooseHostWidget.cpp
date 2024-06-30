// Fill out your copyright notice in the Description page of Project Settings.


#include "ChooseHostWidget.h"
#include "Components/CheckBox.h"
#include "Components/Button.h"
#include "HostScreen.h"
#include "ClientScreen.h"
#include "Team_AIGameMode.h"

void UChooseHostWidget::NativeConstruct()
{
	Super::NativeConstruct();
	OK_Button->OnClicked.AddDynamic(this, &UChooseHostWidget::OkButtonClick);
	if (HostCheck != nullptr)
	{
		HostCheck->OnCheckStateChanged.AddDynamic(this, &UChooseHostWidget::HostCheckBoxClicked);
	}
	if (ClientCheck != nullptr)
	{
		ClientCheck->OnCheckStateChanged.AddDynamic(this, &UChooseHostWidget::ClientCheckBoxClicked);
	}
	bIsHost = false;
}

void UChooseHostWidget::OkButtonClick()
{
	if (bIsHost == true)
	{
		if (HostWidget != nullptr)
		{
			HostScreen = CreateWidget<UHostScreen>(GetWorld(), HostWidget);
			if (HostScreen)
			{
				HostScreen->AddToViewport();
			}
		}
		// host ȭ�� 
	}
	else if (bIsHost == false)
	{
		if (ClientWidget != nullptr)
		{
			ClientScreen = CreateWidget<UClientScreen>(GetWorld(), ClientWidget);
			if (ClientScreen)
			{
				ClientScreen->AddToViewport();
			}
		}
		//client ȭ��
	}
	RemoveFromParent();
}

void UChooseHostWidget::HostCheckBoxClicked(bool bIsCheck)
{
	if (bIsCheck == true)
	{
		if (ClientCheck->IsChecked() == true)
		{
			ClientCheck->CheckedState = ECheckBoxState::Unchecked;
		}
		bIsHost = true;
	}
}

void UChooseHostWidget::ClientCheckBoxClicked(bool bIsCheck)
{
	if (bIsCheck == true)
	{
		if (HostCheck->IsChecked() == true)
		{
			HostCheck->CheckedState = ECheckBoxState::Unchecked;
		}
		bIsHost = false;
	}
}

UHostScreen* UChooseHostWidget::GetHostScreen()
{
	return HostScreen;
}

UClientScreen* UChooseHostWidget::GetClientScreen()
{
	return ClientScreen;
}

UNetworkManager* UChooseHostWidget::GetNetworkManager() const
{
	return GetGameInstance()->GetSubsystem<UNetworkManager>();
}
