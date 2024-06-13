// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AI_Map.h"
#include "GameFramework/Actor.h"
#include "Team_AISpawnPointPlayer.generated.h"

/**
 * 
 */
class AGameCharacter;
UCLASS()
class AI_MAP_API ATeam_AISpawnPointPlayer : public AActor
{
	GENERATED_BODY()
public:

	ATeam_AISpawnPointPlayer();
	UFUNCTION(BlueprintCallable)
	virtual class AGameCharacter* SpawnActor();
	UFUNCTION(BlueprintCallable)
	bool Spawnable() const;
protected:
	virtual void BeginPlay() override;
private:


public:
protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AGameCharacter> PlayerClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector GroundLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bUsed;
private:
};


