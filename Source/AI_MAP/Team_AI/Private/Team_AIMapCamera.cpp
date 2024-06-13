// Fill out your copyright notice in the Description page of Project Settings.


#include "Team_AIMapCamera.h"
#include "Camera/CameraComponent.h"
// Sets default values
ATeam_AIMapCamera::ATeam_AIMapCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MapCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	rotTime2Pi = 10.0f;
	rotTimeCur = 0.0f;
}

// Called when the game starts or when spawned
void ATeam_AIMapCamera::BeginPlay()
{
	Super::BeginPlay();
	FVector2D zeroVec = FVector2D::ZeroVector;
	FVector2D xyVec = FVector2D(GetActorLocation().X, GetActorLocation().Y);
	Distance = (xyVec - zeroVec).Length();
}

// Called every frame
void ATeam_AIMapCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	rotTimeCur += DeltaTime;
	if(rotTimeCur > rotTime2Pi)
		rotTimeCur -= rotTime2Pi;
	//SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FVector::ZeroVector));
	float ratio = rotTimeCur / rotTime2Pi;
	FVector2D xyVec = FVector2D(cos(360 * ratio * PI / 180), sin(360 * ratio * PI / 180)) * Distance;
	SetActorLocation(FVector(xyVec.X, xyVec.Y, GetActorLocation().Z));
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FVector::ZeroVector));
}

