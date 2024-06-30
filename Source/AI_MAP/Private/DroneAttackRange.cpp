// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneAttackRange.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADroneAttackRange::ADroneAttackRange()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	RootComponent = SphereComponent;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(RootComponent);
	Damage = 15.f;
}

// Called when the game starts or when spawned
void ADroneAttackRange::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> OverlapActors;
	auto MyOwner = GetOwner();
	SphereComponent->GetOverlappingActors(OverlapActors);
	if (MyOwner != nullptr)
	{
		auto MyOwnerInstigator = MyOwner->GetInstigatorController();
		auto DamageTypeClass = UDamageType::StaticClass();
		//UGameplayStatics::ApplyDamage(DamagedActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
		for (auto Actor : OverlapActors)
		{
			//UGameplayStatics::ApplyDamage(Actor, Damage, MyOwnerInstigator, this, DamageTypeClass);
			UGameplayStatics::ApplyDamage(Actor, Damage, nullptr, this, DamageTypeClass);
		}
	}
	//Destroy();
}

// Called every frame
void ADroneAttackRange::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

