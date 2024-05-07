// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI_MAP.h"
#include "Team_AISpawnPoint.h"
#include "Team_AISpawnPointPatrol.generated.h"

/**
 * 
 */
UCLASS()
class AI_MAP_API ATeam_AISpawnPointPatrol : public ATeam_AISpawnPoint
{
	GENERATED_BODY()
public:
	ATeam_AISpawnPointPatrol();
	UFUNCTION(BlueprintCallable)
	ATeam_AIPatrolRoute* GetPatrolRoute() const;
	//// Called every frame
	//virtual void Tick(float DeltaTime) override;
	virtual ATeam_AICharacterBase* SpawnActor() override;
	UFUNCTION(BlueprintCallable)
	class ATeam_AICharacterBase* GetSpawnActor();
protected:
private:

public:
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ATeam_AIPatrolRoute* PatrolRoute;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ATeam_AICharacterBase* PatrolActor;
private:

};
