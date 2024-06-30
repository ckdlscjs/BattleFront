// Fill out your copyright notice in the Description page of Project Settings.


#include "Team_AICharacter_Range.h"
#include "Team_AIAnimInstance.h"
#include "Team_AIProjectileBase.h"
#include "Particles/ParticleSystemComponent.h"
#include "Team_AIGameMode.h"

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
			if (!IsValid(this))
				return;
			//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("ProjectileSpawn!")));
			if (ProjectileClass)
			{
				//ActivateParticleSystem(TEXT("Range"));
					
				ATeam_AIProjectileBase* Projectile = Cast<ATeam_AIProjectileBase>(GetWorld()->SpawnActor<AActor>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), GetActorRotation()));
				if (!Projectile)
					return;
				Projectile->SetOwner(this);
				Projectile->ProjectileInitialize(Attack, ProjectileSpeed, ProjectileSpeed, 0.0f);
				Projectile->SetCollisionEnable(true);

				Protocol::C_AIPROJSPAWN projPkt;
				{
					projPkt.set_object_id(pos.object_id());
					auto gm = Cast<ATeam_AIGameMode>(GetWorld()->GetAuthGameMode());
					for (int i = 0; i < gm->GetAIProjClasses().Num(); i++)
					{
						if (ProjectileClass == gm->GetAIProjClasses()[i])
						{
							//i값을 패킷에 저장시킨다.
							projPkt.set_projidx(i);
							break;
						}
					}
					FVector loc = ProjectileSpawnPoint->GetComponentLocation();
					projPkt.set_x(loc.X);
					projPkt.set_y(loc.Y);
					projPkt.set_z(loc.Z);
					FRotator rot = GetActorRotation();
					projPkt.set_yaw(rot.Yaw);
					projPkt.set_pitch(rot.Pitch);
					projPkt.set_roll(rot.Roll);
					projPkt.set_speed(ProjectileSpeed);
					projPkt.set_attackdmg(Attack);

					GetNetworkManager()->SendPacket(projPkt);
				}
				//game mode projectile
				/*
				* client0 - container(projectile ^)
				* clinet_etc - container(projectile_copy,gamemode spawn actor)
				*/
			}
		});
	AnimInstance->OnAttackParticle.AddLambda([this]() -> void
		{
			PlayAudioSystemAtLocation(TEXT("Fire"), GetActorLocation());
			ActivateParticleSystem(TEXT("Range"));
		});
}

void ATeam_AICharacter_Range::BeginPlay()
{
	Super::BeginPlay();
}
