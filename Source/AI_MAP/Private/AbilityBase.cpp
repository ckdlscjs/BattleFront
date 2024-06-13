// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityBase.h"
#include "Protocol.pb.h"
#include "NetworkManager.h"
#include "Components/CapsuleComponent.h"
// Sets default values
AAbilityBase::AAbilityBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MyAbilityLevel = 0;
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

bool AAbilityBase::CheckTime(float DTimer)
{
	
	if (FireTime <= 0.f)
	{
		FireTime = CoolTime;
		return true;
	}
	else
	{
		
		FireTime -= DTimer;
		return false;
	}
	return false;
}


AbilityType AAbilityBase::GetType()
{
	return Type;
}

UTexture2D* AAbilityBase::GetTexture()
{
	return AbilityTexture;
}

void AAbilityBase::SetAbilityLevel(int32 Level)
{
	MyAbilityLevel = Level;
}

int32 AAbilityBase::GetAbilityLevel()
{
	return MyAbilityLevel;
}

FString AAbilityBase::GetAbilityName()
{
	return Name;
}

void AAbilityBase::AbilityLevelUp()
{
	MyAbilityLevel++;
}

UNetworkManager* AAbilityBase::GetNetworkManager() const
{
	return GetGameInstance()->GetSubsystem<UNetworkManager>();
}