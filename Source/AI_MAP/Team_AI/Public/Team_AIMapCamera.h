// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI_Map.h"
#include "GameFramework/Actor.h"
#include "Team_AIMapCamera.generated.h"
class UCameraComponent;
UCLASS()
class AI_MAP_API ATeam_AIMapCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeam_AIMapCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* MapCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Distance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float rotTime2Pi;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float rotTimeCur;
private:

};
