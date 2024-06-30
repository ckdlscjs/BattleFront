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
#include "GameCharacter.h"

#include "Protocol.pb.h"
#include "NetworkManager.h"
#include "GameCharacter.h"	

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
	Damage = 1.0f;
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

void AAbilityChemical::SetLocation(FVector& Location, int abilityIdx)
{
	if (ChemicalSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ChemicalSound, GetActorLocation());
	}
	float X_Min = Location.X - 500.f;
	float X_Max = Location.X + 500.f;
	float Y_Min = Location.Y - 500.f;
	float Y_Max = Location.Y + 500.f;
	float X_Rand = FMath::FRandRange(X_Min, X_Max);
	float Y_Rand = FMath::FRandRange(Y_Min, Y_Max);
	FVector Target(X_Rand, Y_Rand, Location.Z);
	SetActorLocation(Target);

	Protocol::C_PLAYERSKILL_CHEMICAL chemPkt;
	{
		auto Player = Cast<AGameCharacter>(GetOwner());
		chemPkt.set_object_id(Player->PlayerInfo->object_id());
		chemPkt.set_abilityarrayidx(abilityIdx);
		chemPkt.set_x(Target.X);
		chemPkt.set_y(Target.Y);
		chemPkt.set_z(Target.Z);
		chemPkt.set_dmg(this->GetAbilityDetail());
	}
	GetNetworkManager()->SendPacket(chemPkt);
}

void AAbilityChemical::AbilityLevelUp()
{
	Super::AbilityLevelUp();
	CoolTime = 7.f - MyAbilityLevel;
	Damage++;
	Duration += 2;
}

int32 AAbilityChemical::GetProjCount()
{
	return ProjectileCount;
}

float AAbilityChemical::GetAbilityDetail()
{
	return Damage;
}

void AAbilityChemical::SetAbilityDetail(float Details)
{
	Damage = Details;
}

void AAbilityChemical::PlayChemicalSound()
{
	if (ChemicalSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ChemicalSound, GetActorLocation());
	}
}

void AAbilityChemical::TakePlayerDamage()
{
	TArray <AActor*> OverlapActors;
	SphereCollision->GetOverlappingActors(OverlapActors);
	AGameCharacter* MyOwner = Cast<AGameCharacter>(GetOwner());
	if (MyOwner != nullptr)
	{
		auto MyOwnerInstigator = MyOwner->GetInstigatorController();
		auto DamageTypeClass = UDamageType::StaticClass();
		
		for (auto DamagedActor : OverlapActors)
		{
			//UGameplayStatics::ApplyDamage(DamagedActor, Damage, MyOwnerInstigator, MyOwner, DamageTypeClass);
			UGameplayStatics::ApplyDamage(DamagedActor, Damage, nullptr, MyOwner, DamageTypeClass);
		}
	}
}

