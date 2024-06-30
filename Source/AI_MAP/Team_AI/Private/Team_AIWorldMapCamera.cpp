// Fill out your copyright notice in the Description page of Project Settings.


#include "Team_AIWorldMapCamera.h"
#include "Components/SceneCaptureComponent2D.h"
// Sets default values
ATeam_AIWorldMapCamera::ATeam_AIWorldMapCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	SceneCaptureComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATeam_AIWorldMapCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeam_AIWorldMapCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

