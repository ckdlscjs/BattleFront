// Fill out your copyright notice in the Description page of Project Settings.


#include "Team_AISpawnPointBoss.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

ATeam_AISpawnPointBoss::ATeam_AISpawnPointBoss()
{
	MeshDissolveTime = 3.0f;
	DissolveTimeCurrent = 0.0f;
	Fracture = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Fracture"));
	Fracture->SetupAttachment(RootComponent);
}

void ATeam_AISpawnPointBoss::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ATeam_AISpawnPointBoss::BeginDestroy()
{
	Super::BeginDestroy();
	if (GetWorld())
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}


void ATeam_AISpawnPointBoss::BeginPlay()
{
	Super::BeginPlay();
	MaterialDynamicInsts.Add(Fracture->CreateDynamicMaterialInstance(0, Fracture->GetMaterial(0)));
	MaterialDynamicInsts.Add(Fracture->CreateDynamicMaterialInstance(1, Fracture->GetMaterial(1)));

	for(const auto& iter : MaterialDynamicInsts)
		iter->SetScalarParameterValue(FName("GlowValue"), 300.0f);

}

void ATeam_AISpawnPointBoss::SpawnPointDestruction()
{
	Fracture->SetSimulatePhysics(true);
	FVector loc = GetActorLocation();
	auto actor = GetWorld()->SpawnActor(MasterField, &loc);
	//Fracture->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetWorldTimerManager().SetTimer
	(
		SpawnTimerHandle,
		[this]() -> void
		{
			if (!IsValid(this))
				return;
			float ratio = 1.0f - (DissolveTimeCurrent / MeshDissolveTime);
			//material translucen
			for (const auto& iter : MaterialDynamicInsts)
			{
				iter->SetScalarParameterValue(FName("Ratio_Dissolve"), ratio);
				iter->SetScalarParameterValue(FName("Ratio_Intensity"), ratio);
			}

			DissolveTimeCurrent += GetWorld()->GetDeltaSeconds();
			if (DissolveTimeCurrent >= MeshDissolveTime)
			{
				Fracture->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				GetWorldTimerManager().PauseTimer(SpawnTimerHandle);
				Destroy();
			}
		},
		GetWorld()->GetDeltaSeconds(),
		true,
		0.0f
	);
}