// Copyright Epic Games, Inc. All Rights Reserved.

#include "Team_AIGameMode.h"
#include "Team_AIPlayerController.h"
#include "Team_AICharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Team_AISpawnPointRandom.h"
#include "Team_AISpawnPointPatrol.h"
#include "Team_AIPatrolRoute.h"
#include "Team_AICharacterBase.h"
#include "Team_AIGameInstance.h"

ATeam_AIGameMode::ATeam_AIGameMode()
{
	// use our custom PlayerController class

	// set default pawn class to our Blueprinted character

	/*
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("Blueprint'/Game/BP/BP_GameCharacter.BP_GameCharacter_C'"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("Blueprint'/Game/BP/BP_MyController.BP_MyController_C'"));
	if (PlayerControllerBPClass.Class != nullptr)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
	static ConstructorHelpers::FObjectFinder<UBlueprint> blueprint_finder(TEXT("/Game/Level_TeamMap/Blueprint/BP_AICharacterBase.BP_AICharacterBase"));
	if (blueprint_finder.Succeeded())
	{
		mBlueprintClass = (UClass*)blueprint_finder.Object->GeneratedClass;
	}
	*/
	Duration_SpawnEnemyRandom = 7.0f;
	Duration_SpawnEnemyPatrol = 10.0f;
	Duration_GameLevel = 15.0f;
	GameLevel = 1;
}


void ATeam_AIGameMode::StartPlay()
{
	Super::StartPlay();
	bAllowTickBeforeBeginPlay = true;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATeam_AISpawnPointRandom::StaticClass(), SpawnPoints_Random);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATeam_AIPatrolRoute::StaticClass(), PatrolRoutes);

	//IncreaseGameLevel
	GetWorldTimerManager().SetTimer
	(
		TimerHandle_GameLevel,
		[this]() -> void
		{
			UTeam_AIGameInstance* GameInstance = Cast<UTeam_AIGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
			GameLevel = std::min(GameLevel + 1, GameInstance->GetSizeCharacterDataTable());
			if (GameLevel >= GameInstance->GetSizeCharacterDataTable())
				GetWorldTimerManager().ClearTimer(TimerHandle_GameLevel);
		}, 
		Duration_GameLevel,
		true

	);

	//SpawnEnemy
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnRandom, this, &ATeam_AIGameMode::SpawnEnemyRandom, Duration_SpawnEnemyRandom, true);
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnPatrol, this, &ATeam_AIGameMode::SpawnEnemyPatrol, Duration_SpawnEnemyPatrol, true);
}

void ATeam_AIGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}



void ATeam_AIGameMode::SpawnEnemyRandom()
{
	if (SpawnPoints_Random.Num() <= 0)
		return;
	//TODO : Random Enemy
	for (const auto& spawnPoint : SpawnPoints_Random)
	{
		AddSpawnActor(Cast<ATeam_AISpawnPointRandom>(spawnPoint)->SpawnActor());
	}
}

void ATeam_AIGameMode::SpawnEnemyPatrol()
{
	if (PatrolRoutes.Num() <= 0)
		return;
	//TODO : Patrol Enemy
	for (const auto& iter : PatrolRoutes)
	{
		ATeam_AIPatrolRoute* patrolRoute = Cast<ATeam_AIPatrolRoute>(iter);
		if (patrolRoute->CheckSpawnPossible())
		{
			for (const auto& spawnPoint : patrolRoute->GetSpawnPoints())
			{
				AddSpawnActor(spawnPoint->SpawnActor());
			}
		}
	}
}

int32 ATeam_AIGameMode::GetGameLevel() const
{
	return GameLevel;
}

void ATeam_AIGameMode::AddSpawnActor(AActor* actor)
{
	if (!actor)
		return;
	Enemys.Add(actor);
}

void ATeam_AIGameMode::DeleteSpawnActor(AActor* actor)
{
	if (!actor)
		return;
	Enemys.Remove(actor);
}

float ATeam_AIGameMode::GetDurationSpawnEnemyRandom() const
{
	return Duration_SpawnEnemyRandom;
}

float ATeam_AIGameMode::GetDurationSpawnEnemyPatrol() const
{
	return Duration_SpawnEnemyPatrol;
}
