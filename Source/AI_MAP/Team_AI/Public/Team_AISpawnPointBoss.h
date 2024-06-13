// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI_Map.h"
#include "Team_AISpawnPoint.h"
#include "Team_AISpawnPointBoss.generated.h"

/**
 * 
 */
class UGeometryCollectionComponent;
class AFieldSystemActor;
UCLASS()
class AI_MAP_API ATeam_AISpawnPointBoss : public ATeam_AISpawnPoint
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ATeam_AISpawnPointBoss();
	virtual void PostInitializeComponents() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void SpawnPointDestruction() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGeometryCollectionComponent* Fracture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> MasterField;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UMaterialInstanceDynamic*> MaterialDynamicInsts;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MeshDissolveTime;
	float DissolveTimeCurrent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)

	FTimerHandle SpawnTimerHandle;
};
