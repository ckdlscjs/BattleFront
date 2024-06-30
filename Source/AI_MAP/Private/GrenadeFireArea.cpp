// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeFireArea.h"
#include "Components/SphereComponent.h"
#include "GameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/EngineTypes.h"
#include "GameCharacter.h"
// Sets default values
AGrenadeFireArea::AGrenadeFireArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	RootComponent = SphereCollision;
	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Fire Particle"));
	ParticleComponent->SetupAttachment(RootComponent);
	DurationTime = 5.f;
	Damage = 3.f;
	RateToTime = 1.f;
}

// Called when the game starts or when spawned
void AGrenadeFireArea::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &AGrenadeFireArea::TakePlayerDamage, RateToTime, true);
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
}

// Called every frame
void AGrenadeFireArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DurationTime -= DeltaTime;
	if (DurationTime <= 0.f)
	{
		GetWorldTimerManager().ClearTimer(DamageTimerHandle);
		Destroy();
	}
}

void AGrenadeFireArea::SetFireDamage(float Amount)
{
	Damage = Amount;
}

void AGrenadeFireArea::TakePlayerDamage()
{
	AGameCharacter* MyOwner = Cast<AGameCharacter>(GetOwner());
	SphereCollision->GetOverlappingActors(OverlapActors);
	if (MyOwner != nullptr)
	{
		auto MyOwnerInstigator = MyOwner->GetInstigatorController();
		auto DamageTypeClass = UDamageType::StaticClass();
		
		//UGameplayStatics::ApplyDamage(DamagedActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
		for (auto Actor : OverlapActors)
		{
			//UGameplayStatics::ApplyDamage(Actor, Damage, MyOwnerInstigator, MyOwner, DamageTypeClass);
			UGameplayStatics::ApplyDamage(Actor, Damage, nullptr, MyOwner, DamageTypeClass);
		}
	}
}
