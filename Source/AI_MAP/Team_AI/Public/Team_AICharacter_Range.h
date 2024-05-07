// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Team_AICharacterBase.h"
#include "Team_AICharacter_Range.generated.h"

/**
 * 
 */
UCLASS()
class AI_MAP_API ATeam_AICharacter_Range : public ATeam_AICharacterBase
{
	GENERATED_BODY()
public:
	ATeam_AICharacter_Range();
	virtual void PostInitializeComponents() override;
	// Called to bind functionality to input

	virtual void BeginPlay() override;
protected:

private:



public:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* ProjectileSpawnPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ATeam_AIProjectileBase> ProjectileClass;
private:
	
};
