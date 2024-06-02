// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityChemical.h"
#include "Components/CapsuleComponent.h" 
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Particles/ParticleSystemComponent.h"

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
	Damage = 0.0f;
	CoolTime = 7.f - MyAbilityLevel;
	ProjectileCount = 1;
	Type = AbilityType::Range;
	//ProjectileMovement->ProjectileGravityScale = 0.0f;
	ChemicalDamage = 1.0f;
	Duration = 10.f;
}

void AAbilityChemical::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &AAbilityChemical::TakePlayerDamage, 0.5f, true);
	//SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AAbilityChemical::ProjectileBeginOverlap);
	//SphereCollision->OnComponentEndOverlap.AddDynamic(this, &AGrenadeFireArea::EndOverlap);

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


void AAbilityChemical::ProjectileBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//auto MyOwner = GetOwner();
	//if (MyOwner != nullptr)
	//{

	//	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	//	{
	//		//DamagedActor = OtherActor;		
	//		GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &AAbilityChemical::TakePlayerDamage, 0.5f, true);
	//	}
	//}
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

void AAbilityChemical::TakePlayerDamage()
{
	TArray <AActor*> OverlapActors;
	SphereCollision->GetOverlappingActors(OverlapActors);
	
	auto MyOwner = GetOwner();
	if (MyOwner != nullptr)
	{
		auto MyOwnerInstigator = MyOwner->GetInstigatorController();
		auto DamageTypeClass = UDamageType::StaticClass();
		//UGameplayStatics::ApplyDamage(DamagedActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
		for (auto DamagedActor : OverlapActors)
		{
			UGameplayStatics::ApplyDamage(DamagedActor, ChemicalDamage, MyOwnerInstigator, this, DamageTypeClass);
		}
	}
}

