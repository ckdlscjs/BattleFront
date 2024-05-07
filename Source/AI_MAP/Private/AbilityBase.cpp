// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Misc/App.h"
// Sets default values
AAbilityBase::AAbilityBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	AbilityLevel = 1;
	FireTime = 0.f;

}

// Called when the game starts or when spawned
void AAbilityBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAbilityBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


bool AAbilityBase::CheckTime()
{
	
	if (FireTime <= 0.f)
	{
		FireTime = CoolTime;
		return true;
	}
	else
	{
		
		FireTime -= FApp::GetDeltaTime();
		return false;
	}
	return false;
}

int32 AAbilityBase::GetProjCount()
{
	return ProjectileCount;
}

AbilityType AAbilityBase::GetType()
{
	return Type;
}

