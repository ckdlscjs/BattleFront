// Fill out your copyright notice in the Description page of Project Settings.


#include "Team_AICharacter_Melee.h"
#include "Team_AIAnimInstance.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "NetworkManager.h"

ATeam_AICharacter_Melee::ATeam_AICharacter_Melee()
{
	AttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollision"));
	AttackCollision->SetupAttachment(RootComponent);
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATeam_AICharacter_Melee::BeginPlay()
{
	Super::BeginPlay();
	AttackRange = AttackCollision->GetRelativeLocation().X;

}

void ATeam_AICharacter_Melee::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ATeam_AICharacter_Melee::AttackCollisionOverlap);
	if (!AnimInstance)
		return;
	AnimInstance->OnAttackStart.AddLambda([this]() -> void
		{
			if (!IsValid(this))
				return;
			//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("LambdaAttack!")));
			AttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

			//Protocol::C_AIATTACK aiAttackPkt;
			//{
			//	aiAttackPkt.set_object_id(pos.object_id());
			//	GetNetworkManager()->SendPacket(aiAttackPkt);
			//}
		});
	AnimInstance->OnAttackEnd.AddLambda([this]() -> void
		{
			if (!IsValid(this))
				return;
			//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("AttackEnd!")));
			AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		});
	AnimInstance->OnAttackParticle.AddLambda([this]() -> void
		{
			if (!IsValid(this))
				return;
			PlayAudioSystemAtLocation(TEXT("Slash"), GetActorLocation());
			ActivateParticleSystem(TEXT("Melee"));
		});
}

void ATeam_AICharacter_Melee::AttackCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO:BeginOverlap
	/*if (AttackParticleSystem->Template)
		AttackParticleSystem->Activate(true);*/
	UGameplayStatics::ApplyDamage(OtherActor, Attack, nullptr, this, UDamageType::StaticClass());
	//TODO : Make Hit Packet
	// need object_id
	Protocol::C_AIHIT aiHitPkt;
	{
		aiHitPkt.set_object_id(pos.object_id());
		GetNetworkManager()->SendPacket(aiHitPkt);
	}
}
