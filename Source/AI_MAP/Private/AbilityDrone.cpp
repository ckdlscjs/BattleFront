// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityDrone.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "DroneAttackRange.h"
#include "DrawDebugHelpers.h"
#include "AI_MAP.h"
AAbilityDrone::AAbilityDrone()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SphereComponent->SetGenerateOverlapEvents(false);
	RootComponent = SphereComponent;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(RootComponent);
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);
	MoveComponent = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("Move Component"));
	Speed = 1500.f;
	bAttack = false;
	Damage = 5.f;
	MyAbilityLevel = 0;
	CoolTime = 8.f;
	Type = AbilityType::Drone;
}

void AAbilityDrone::BeginPlay()
{
	Super::BeginPlay();
}

void AAbilityDrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	MY_DeltaTime = DeltaTime;

	if (bAttack == true)
	{
		if (MoveToTarget())
		{
			UKismetSystemLibrary::PrintString(GetWorld(), FString(TEXT("Shoot")));
			FVector Loc = GetActorLocation();
			Loc.Z = 0.f;
			UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, Loc, 200.f, nullptr, TArray<AActor*>(),this,false, true);
			//DrawDebugSphere(GetWorld(), Loc, 200.f, 26, FColor(181, 0, 0), true, -1, 0, 2);

			bAttack = false;
		}
	}
}

void AAbilityDrone::SetLocation(FVector& Location)
{
	StartLocation = GetActorLocation();
	float X_Min = Location.X - 500.f;
	float X_Max = Location.X + 500.f;
	float Y_Max = Location.Y + 500.f;
	float Y_Min = Location.Y - 500.f;
	float X_Rand = FMath::FRandRange(X_Min, X_Max);
	float Y_Rand = FMath::FRandRange(Y_Min, Y_Max);
	TargetLocation = FVector(X_Rand, Y_Rand, Location.Z);	
	TargetDistance = (TargetLocation - StartLocation).Size();
	//DrawDebugSphere(GetWorld(), TargetLocation, 200, 26, FColor(181, 0, 0), true, -1, 0, 2);
	Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
	bAttack = true;
	CurrentDistance = 0.f;
}


void AAbilityDrone::FindEnemy(TArray<AActor*> Actors)
{

}

bool AAbilityDrone::MoveToTarget()
{
	if (TargetDistance > CurrentDistance)
	{
		FVector CurrentLocation = GetActorLocation();
		CurrentLocation += Direction * Speed * MY_DeltaTime;
		SetActorLocation(CurrentLocation);
		CurrentDistance = (CurrentLocation - StartLocation).Size();
		return false;
	}	
	return true;
}

void AAbilityDrone::ChangeAttackStatus(bool bChange)
{
	bAttack = bChange;
}

bool AAbilityDrone::GetAttackState()
{
	return bAttack;
}
