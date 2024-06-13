// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityChemical.h"
#include "Components/CapsuleComponent.h" 
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

AAbilityChemical::AAbilityChemical()
{
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	RootComponent = SphereCollision;
	//StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	//StaticMesh->SetupAttachment(RootComponent);
	ChemicalParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	ChemicalParticle->SetupAttachment(RootComponent);
	MyAbilityLevel = 0;
	CoolTime = 7.f - MyAbilityLevel;
	ProjectileCount = 1;
	Type = AbilityType::Range;
	ChemicalDamage = 1.0f;
	Duration = 10.f;
}

void AAbilityChemical::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &AAbilityChemical::TakePlayerDamage, 1.5f, true);
}

void AAbilityChemical::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Duration -= DeltaTime;
	if (Duration <= 0.f)
	{
		GetWorldTimerManager().ClearTimer(DamageTimerHandle);
		Destroy();
	}
}

void AAbilityChemical::SetLocation(FVector& Location)
{
	float X_Min = Location.X - 500.f;
	float X_Max = Location.X + 500.f;
	float Y_Min = Location.Y - 500.f;
	float Y_Max = Location.Y + 500.f;
	float X_Rand = FMath::FRandRange(X_Min, X_Max);
	float Y_Rand = FMath::FRandRange(Y_Min, Y_Max);
	FVector Target(X_Rand, Y_Rand, Location.Z);
	SetActorLocation(Target);
}

void AAbilityChemical::AbilityLevelUp()
{
	Super::AbilityLevelUp();
	CoolTime = 7.f - MyAbilityLevel;
	ChemicalDamage++;
	Duration += 2;
}

int32 AAbilityChemical::GetProjCount()
{
	return ProjectileCount;
}

void AAbilityChemical::TakePlayerDamage()
{
	TArray <AActor*> OverlapActors;
	SphereCollision->GetOverlappingActors(OverlapActors);
	
	auto MyOwner = GetOwner();
	if (MyOwner != nullptr)
	{
		auto MyOwnerInstigator = MyOwner->GetInstigatorController();
		auto DamageTypeClass = UDamageType::StaticClass();
		
		for (auto DamagedActor : OverlapActors)
		{
			UGameplayStatics::ApplyDamage(DamagedActor, ChemicalDamage, MyOwnerInstigator, this, DamageTypeClass);
		}
	}
}

