// Copyright Epic Games, Inc. All Rights Reserved.

#include "AI_MAPGameMode.h"
#include "AI_MAPPlayerController.h"
#include "AI_MAPCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAI_MAPGameMode::AAI_MAPGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AAI_MAPPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}