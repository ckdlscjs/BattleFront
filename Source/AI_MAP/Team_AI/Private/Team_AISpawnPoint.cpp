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
}

// Called every frame
void ATeam_AISpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ATeam_AICharacterBase* ATeam_AISpawnPoint::SpawnActor()
{	
	//TODO : Random Enemy
	//
	auto spawnActor = GetWorld()->SpawnActor<ATeam_AICharacterBase>(EnemyClass, GetActorLocation(), { 0, 0, 0 });
	//send to packet server
	if (!IsValid(spawnActor))
	{
		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("NotActorSpawn")));
		return nullptr;
	}
	spawnActor->pos.set_object_id(AIID++);
	return spawnActor;
}

ATeam_AICharacter_Recv* ATeam_AISpawnPoint::RecvedSpawnActor()
{
	auto loc = GetActorLocation();
	auto temp = GetWorld()->SpawnActor(AIClassRecv, &loc);
	auto spawnActor = Cast<ATeam_AICharacter_Recv>(temp);
	//auto spawnActor = GetWorld()->SpawnActor(AIClassRecv, GetActorLocation());
	if (!IsValid(spawnActor))
	{
		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("NotRecvedActorSpawn")));
		return nullptr;
	}
	//spawnActor->pos.set_object_id(AiRandomSpawnPkt.object_id());
	/*auto temp = Cast<ATeam_AICharacter_Recv>(spawnActor);*/
	return spawnActor;
}
