// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI_Map.h"
#include "GameFramework/Actor.h"
#include "Team_AIWorldMapCamera.generated.h"

class USceneCaptureComponent2D;

UCLASS()
class AI_MAP_API ATeam_AIWorldMapCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeam_AIWorldMapCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneCaptureComponent2D* SceneCaptureComponent;
private:
};
