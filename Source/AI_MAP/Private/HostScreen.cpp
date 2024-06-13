// Fill out your copyright notice in the Description page of Project Settings.


#include "HostScreen.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
void UHostScreen::NativeConstruct()
{
	Super::NativeConstruct();
	GameStartButton->OnClicked.AddDynamic(this, &UHostScreen::GameStartButtonClick);
}

void UHostScreen::GameStartButtonClick()
{
	//gamestart logic
}

void UHostScreen::SetPlayerCount(int32 PlayerCount)
{
	FText Player = FText::FromString(FString::FromInt(PlayerCount));
	PlayerNum->SetText(Player);
}
