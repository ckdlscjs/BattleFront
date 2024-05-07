// Fill out your copyright notice in the Description page of Project Settings.


#include "Team_AISpawnPointPatrol.h"
#include "Team_AIPatrolRoute.h"
#include "Team_AIGameMode.h"
#include "Team_AICharacterBase.h"

ATeam_AISpawnPointPatrol::ATeam_AISpawnPointPatrol()
{
}

ATeam_AIPatrolRoute* ATeam_AISpawnPointPatrol::GetPatrolRoute() const
{
	return PatrolRoute;
}

//void ATeam_AISpawnPointPatrol::Tick(float DeltaTime)
//{
//}


ATeam_AICharacterBase* ATeam_AISpawnPointPatrol::SpawnActor()
{
	//TODO : Random Enemy
	PatrolActor = Cast<ATeam_AICharacterBase>(Super::SpawnActor());
	if(PatrolActor)
		PatrolActor->SetPatrolRoute(PatrolRoute);
	return PatrolActor;
}

ATeam_AICharacterBase* ATeam_AISpawnPointPatrol::GetSpawnActor()
{
	return PatrolActor;
}
