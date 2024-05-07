// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI_MAP.h"
#include "GameFramework/Actor.h"
#include "TEAM_AISpawnPoint.generated.h"
UCLASS()
class AI_MAP_API ATeam_AISpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeam_AISpawnPoint();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	virtual class ATeam_AICharacterBase* SpawnActor();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	
public:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ATeam_AICharacterBase> EnemyClass;
private:
};
