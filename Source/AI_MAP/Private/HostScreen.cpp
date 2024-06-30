// Fill out your copyright notice in the Description page of Project Settings.


#include "HostScreen.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "NetworkManager.h"
#include "Kismet/GameplayStatics.h"
#include "Team_AIGameMode.h"
#include "RunExternalExe.h"

void UHostScreen::NativeConstruct()
{
	Super::NativeConstruct();
	GameStartButton->OnClicked.AddDynamic(this, &UHostScreen::GameStartButtonClick);

	GetNetworkManager()->externalExe->RunExternalExeFile();
	FString localIP = GetNetworkManager()->GetLocalIPAddress();
	GetNetworkManager()->ConnectToGameServer(localIP);
}

void UHostScreen::GameStartButtonClick()
{
	//gamestart logic
	auto gm = Cast<ATeam_AIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gm->GameStart();
}

void UHostScreen::SetPlayerCount(int32 PlayerCount)
{
	FText Player = FText::FromString(FString::FromInt(PlayerCount));
	PlayerNum->SetText(Player);
}

UNetworkManager* UHostScreen::GetNetworkManager() const
{
	return GetGameInstance()->GetSubsystem<UNetworkManager>();
}
