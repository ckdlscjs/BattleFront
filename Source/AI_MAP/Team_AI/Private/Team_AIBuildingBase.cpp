// Fill out your copyright notice in the Description page of Project Settings.


#include "Team_AIBuildingBase.h"
#include "Components/BoxComponent.h"

// Sets default values
ATeam_AIBuildingBase::ATeam_AIBuildingBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;

	Roof = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Roof"));
	Roof->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATeam_AIBuildingBase::BeginPlay()
{
	Super::BeginPlay();
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ATeam_AIBuildingBase::ActorBeginOverlap);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ATeam_AIBuildingBase::ActorEndOverlap);
}

// Called every frame
void ATeam_AIBuildingBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATeam_AIBuildingBase::ActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("BuildingOverlap!!")), true, false, FColor::Black);
	Roof->SetVisibility(false);
}

void ATeam_AIBuildingBase::ActorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Roof->SetVisibility(true);

}

