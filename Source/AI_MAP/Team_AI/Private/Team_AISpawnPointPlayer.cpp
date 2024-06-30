// Fill out your copyright notice in the Description page of Project Settings.


#include "Team_AISpawnPointPlayer.h"
#include "Team_AIGameMode.h"
#include "GameCharacter.h"

ATeam_AISpawnPointPlayer::ATeam_AISpawnPointPlayer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));
	RootComponent = Mesh;
	bUsed = false;
}

AGameCharacter* ATeam_AISpawnPointPlayer::SpawnActor()
{
	AGameCharacter* spawnActor = nullptr;
	spawnActor = GetWorld()->SpawnActor<AGameCharacter>(PlayerClass, GetActorLocation(), { 0, 0, 0 });
	if (!spawnActor)
		return spawnActor;
	bUsed = true;
	GroundLocation.Z += spawnActor->GetDefaultHalfHeight();
	spawnActor->SetActorLocation(GroundLocation);
	//SpawnPointDestruction();
	//send to packet server
	if (!IsValid(spawnActor))
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("NotActorSpawn")));
		return nullptr;
	}
	return spawnActor;
}

bool ATeam_AISpawnPointPlayer::Spawnable() const
{
	return !bUsed;
}

void ATeam_AISpawnPointPlayer::BeginPlay()
{
	Super::BeginPlay();
	FHitResult result;
	FVector downVector(0.0f, 0.0f, -100000.0f);
	GetWorld()->LineTraceSingleByProfile(result, GetActorLocation(), downVector, FName(TEXT("BlockAll")));
	GroundLocation = result.Location;
}
