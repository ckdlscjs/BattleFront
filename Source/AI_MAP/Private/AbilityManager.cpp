// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityManager.h"
#include "AbilityBase.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AAbilityManager::AAbilityManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AAbilityManager::Attack(FVector& Loc)
{
	for (auto AbilityData : AbilityArray)
	{
		AbilityType Type = AbilityData->GetType();
		if (Type == AbilityType::Range)
		{
			AbilityData->Attack(Loc);
		}	
	}
}

// Called when the game starts or when spawned
void AAbilityManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAbilityManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

