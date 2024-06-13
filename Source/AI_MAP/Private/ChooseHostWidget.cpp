// Fill out your copyright notice in the Description page of Project Settings.


#include "ChooseHostWidget.h"
#include "Components/CheckBox.h"
#include "Components/Button.h"
#include "HostScreen.h"
#include "ClientScreen.h"

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
	RemoveFromParent();
	if (bIsHost == true)
	{
		if (HostWidget != nullptr)
		{
			UHostScreen* HostScreen = CreateWidget<UHostScreen>(GetWorld(), HostWidget);
			if (HostScreen)
			{
				HostScreen->AddToViewport();
			}
		}
		// host 화면 
	}
	else if (bIsHost == false)
	{
		if (ClientWidget != nullptr)
		{
			UClientScreen* ClientScreen = CreateWidget<UClientScreen>(GetWorld(), ClientWidget);
			if (ClientScreen)
			{
				ClientScreen->AddToViewport();
			}
		}
		//client 화면
	}
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
