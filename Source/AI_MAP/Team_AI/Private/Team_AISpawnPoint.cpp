// Fill out your copyright notice in the Description page of Project Settings.


#include "Team_AISpawnPoint.h"
#include "Team_AICharacterBase.h"
#include "Team_AIGameMode.h"
#include "Team_AICharacter_Recv.h"


// Sets default values
ATeam_AISpawnPoint::ATeam_AISpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void ATeam_AISpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	FHitResult result;
	FVector downVector(0.0f, 0.0f, -100000.0f);
	GetWorld()->LineTraceSingleByProfile(result, GetActorLocation(), downVector, FName(TEXT("BlockAll")));
	GroundLocation = result.Location;
}

void ATeam_AISpawnPoint::SpawnPointDestruction()
{
}

// Called every frame
void ATeam_AISpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ATeam_AICharacterBase* ATeam_AISpawnPoint::SpawnActor()
{	
	ATeam_AICharacterBase* spawnActor = nullptr;
	spawnActor = GetWorld()->SpawnActor<ATeam_AICharacterBase>(EnemyClass, GetActorLocation(), { 0, 0, 0 });
	if (!spawnActor)
		return spawnActor;
	GroundLocation.Z += spawnActor->GetDefaultHalfHeight();
	spawnActor->SetActorLocation(GroundLocation);
	SpawnPointDestruction();
	//send to packet server
	if (!IsValid(spawnActor))
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("NotActorSpawn")));
		return nullptr;
	}
	spawnActor->pos.set_object_id(AIID++);
	return spawnActor;
}

ATeam_AICharacter_Recv* ATeam_AISpawnPoint::RecvedSpawnActor()
{
	ATeam_AICharacter_Recv* spawnActor = nullptr;
	spawnActor = GetWorld()->SpawnActor<ATeam_AICharacter_Recv>(AIClassRecv, GetActorLocation(), { 0, 0, 0 });
	if (!spawnActor)
		return spawnActor;
	GroundLocation.Z += spawnActor->GetDefaultHalfHeight();
	spawnActor->SetActorLocation(GroundLocation); //NOT KNOW TO NEEDED.......................
	SpawnPointDestruction();
	if (!IsValid(spawnActor))
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("NotRecvedActorSpawn")));
		return nullptr;
	}
	return spawnActor;
}
