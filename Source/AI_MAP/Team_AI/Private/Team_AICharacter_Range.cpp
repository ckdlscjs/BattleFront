// Fill out your copyright notice in the Description page of Project Settings.


#include "Team_AICharacter_Range.h"
#include "Team_AIAnimInstance.h"
#include "Team_AIProjectileBase.h"
#include "Particles/ParticleSystemComponent.h"

ATeam_AICharacter_Range::ATeam_AICharacter_Range()
{
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);
}

void ATeam_AICharacter_Range::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance->OnAttackStart.AddLambda([this]() -> void
		{
			UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("ProjectileSpawn!")));
			if (ProjectileClass)
			{
				if (AttackParticleSystem->Template)
					AttackParticleSystem->Activate(true);
					
				ATeam_AIProjectileBase* Projectile = Cast<ATeam_AIProjectileBase>(GetWorld()->SpawnActor<AActor>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), GetActorRotation()));
				if (!Projectile)
					return;
				Projectile->SetOwner(this);
				Projectile->ProjectileInitialize(Attack, ProjectileSpeed, ProjectileSpeed, 0.0f);
				Projectile->SetCollisionEnable(true);
			}
		});
}

void ATeam_AICharacter_Range::BeginPlay()
{
	Super::BeginPlay();
}
