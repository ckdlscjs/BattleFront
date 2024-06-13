// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityDrone.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DroneAttackRange.h"
#include "DrawDebugHelpers.h"
#include "AI_MAP.h"
#include "Team_AIGameMode.h"
#include "GameCharacter.h"
#include "Particles/ParticleSystemComponent.h"

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
	DroneParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Component"));
	DroneParticleSystemComponent->SetupAttachment(RootComponent);
	Speed = 1500.f;
	bAttack = false;
	Damage = 5.f;
	MyAbilityLevel = 0;
	CoolTime = 8.f;
	Type = AbilityType::Drone;
	State = DroneState::None;
	
}

void AAbilityDrone::BeginPlay()
{
	Super::BeginPlay();
	DroneParticleSystemComponent->bAutoActivate = false;
}

void AAbilityDrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MY_DeltaTime = DeltaTime;

	if (Cast<AGameCharacter>(GetOwner()) == Cast<ATeam_AIGameMode>(GetWorld()->GetAuthGameMode())->GetMyPlayer())
	{
		if (State == DroneState::Search)
		{	
			if (MoveToTarget())
			{
				Attack();
			}
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
	if (State != DroneState::Return)
	{
		State = DroneState::Search;
		TargetLocation = FVector(X_Rand, Y_Rand, StartLocation.Z);
	}
	else if (State == DroneState::Return)
	{
		TargetLocation = Location;
	}
	
	
	TargetDistance = (TargetLocation - StartLocation).Size();
	//DrawDebugSphere(GetWorld(), TargetLocation, 200, 26, FColor(181, 0, 0), true, -1, 0, 2);
	Direction = (TargetLocation - StartLocation).GetSafeNormal();


	CurrentDistance = 0.f;
}

DroneState AAbilityDrone::GetDroneState()
{
	return State;
}

void AAbilityDrone::SetDroneStateReturn()
{
	State = DroneState::Return;
}

void AAbilityDrone::ReturnDrone(FVector& Location)
{

}

void AAbilityDrone::SetDroneNoneState()
{
	State = DroneState::None;
}

void AAbilityDrone::AbilityLevelUp()
{
	Super::AbilityLevelUp();
	Damage += 2;
	CoolTime -= MyAbilityLevel;
}

void AAbilityDrone::SetDroneRotation()
{
	FVector start = GetActorForwardVector();
	FVector target = TargetLocation - GetActorLocation();
	FRotator test = target.Rotation();
	target.Z = start.Z;
	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(start, target);
	SetActorRotation(test);
}

void AAbilityDrone::Attack()
{
	FVector Loc = GetActorLocation();
	Loc.Z = 0.f;
	if (DroneParticleSystemComponent->Template)
	{
		DroneParticleSystemComponent->SetWorldLocation(Loc);
		DroneParticleSystemComponent->Activate(true);
	}
		
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, Loc, 200.f, nullptr, TArray<AActor*>(), this, false, true);
}

bool AAbilityDrone::MoveToTarget()
{
	if (TargetDistance > CurrentDistance)
	{
	
		
		FVector CurrentLocation = GetActorLocation();
		CurrentLocation += Direction * Speed * MY_DeltaTime;
		SetActorLocation(CurrentLocation);
		CurrentDistance = (CurrentLocation - StartLocation).Size();

		//TODO : MoveDronePkt;
		// OwnerID, CurrentLocation

		Protocol::C_MOVEDRONE drnMovePkt;
		{
			auto player = Cast<AGameCharacter>(GetOwner());
			drnMovePkt.set_object_id(player->PlayerInfo->object_id());
			drnMovePkt.set_x(CurrentLocation.X);
			drnMovePkt.set_y(CurrentLocation.Y);
			drnMovePkt.set_z(CurrentLocation.Z);
		}
		GetNetworkManager()->SendPacket(drnMovePkt);

		return false;
	}
	else
	{
		if (State != DroneState::Return)
		{
			State = DroneState::Attack;
		}
		else
		{
			State = DroneState::Search;
		}
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
