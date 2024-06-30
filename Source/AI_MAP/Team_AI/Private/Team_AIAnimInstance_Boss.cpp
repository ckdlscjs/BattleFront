// Fill out your copyright notice in the Description page of Project Settings.


#include "Team_AIAnimInstance_Boss.h"
#include "Team_AICharacter_Boss.h"
#include "GameCharacter.h"
#include "NetworkManager.h"
#include "Team_AIGameMode.h"
#include "Particles/ParticleSystemComponent.h"

UTeam_AIAnimInstance_Boss::UTeam_AIAnimInstance_Boss()
{
	Attack4Delay = 1.0f;
}

void UTeam_AIAnimInstance_Boss::BeginDestroy()
{
	Super::BeginDestroy();
	if (GetWorld())
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void UTeam_AIAnimInstance_Boss::AnimNotify_Attack1StartCheck()
{
	IsAttacking = true;
	auto Boss = Cast<ATeam_AICharacter_Boss>(OwnerCharacter);
	if (!Boss)
		return;
	Boss->PlayAudioSystemAtLocation(TEXT("SoundAttack1"), Boss->GetActorLocation());
	Boss->SetAmountKnockback(0.0f);
	Boss->SetMeleeAttackCollision(true);
}

void UTeam_AIAnimInstance_Boss::AnimNotify_Attack1EndCheck()
{
	IsAttacking = false;
	auto Boss = Cast<ATeam_AICharacter_Boss>(OwnerCharacter);
	if (!Boss)
		return;
	Boss->SetAmountKnockback(0.0f);
	Boss->SetMeleeAttackCollision(false);
}
void UTeam_AIAnimInstance_Boss::AnimNotify_Attack2StartCheck()
{
	IsAttacking = true;
	//Send to Server Players(Tarray<AActor*>), CleanContainerData, InsertData
	auto Boss = Cast<ATeam_AICharacter_Boss>(OwnerCharacter);
	if (!Boss)
		return;
	
	auto gm = Cast<ATeam_AIGameMode>(GetWorld()->GetAuthGameMode());
	if (gm->GetMyPlayer()->PlayerInfo->object_id() == 1)
	{
		Protocol::C_AIATTACK_BOSS2 bossAttack2Pkt;
		bossAttack2Pkt.set_object_id(Boss->pos.object_id());
		for (auto player : Boss->GetRecognizePlayers())
		{
			auto p = Cast<AGameCharacter>(player);
			bossAttack2Pkt.add_target_id(p->PlayerInfo->object_id());
		}
		auto nm = Boss->GetNetworkManager();
		nm->SendPacket(bossAttack2Pkt);
	}
}

void UTeam_AIAnimInstance_Boss::AnimNotify_Attack2EndCheck()
{
	IsAttacking = false;
}

void UTeam_AIAnimInstance_Boss::AnimNotify_Attack3StartCheck()
{
	IsAttacking = true;
	auto Boss = Cast<ATeam_AICharacter_Boss>(OwnerCharacter);
	if (!Boss)
		return;
	Boss->SetAmountKnockback(5000.0f);
	Boss->SetMeleeAttackCollision(true);
}

void UTeam_AIAnimInstance_Boss::AnimNotify_Attack3EndCheck()
{
	IsAttacking = false;
	auto Boss = Cast<ATeam_AICharacter_Boss>(OwnerCharacter);
	if (!Boss)
		return;
	Boss->SetAmountKnockback(0.0f);
	Boss->SetMeleeAttackCollision(false);
}

void UTeam_AIAnimInstance_Boss::AnimNotify_Attack4SoundStart()
{
	OwnerCharacter->PlayAudioSystemAtLocation(TEXT("SoundAttack4Start"), OwnerCharacter->GetActorLocation());
}

void UTeam_AIAnimInstance_Boss::AnimNotify_Attack4StartCheck()
{
	IsAttacking = true;
	GetWorld()->GetTimerManager().SetTimer
	(
		BossAttackTimerHandle,
		[this]() -> void
		{
			if (!IsValid(this))
				return;
			OwnerCharacter->PlayAudioSystemAtLocation(TEXT("SoundAttack4"), OwnerCharacter->GetActorLocation());
			OwnerCharacter->ActivateParticleSystem(TEXT("Attack4"));
			auto Boss = Cast<ATeam_AICharacter_Boss>(OwnerCharacter);
			if (!Boss)
				return;
			Boss->Attack4();
		},
		Attack4Delay,
		true,
		0.0f
	);
}

void UTeam_AIAnimInstance_Boss::AnimNotify_Attack4EndCheck()
{
	IsAttacking = false;
	GetWorld()->GetTimerManager().ClearTimer(BossAttackTimerHandle);
}

void UTeam_AIAnimInstance_Boss::AnimNotify_Attack2ParticleCheck()
{
	if (!OwnerCharacter)
		return;
	OwnerCharacter->PlayAudioSystemAtLocation(TEXT("SoundAttack2"), OwnerCharacter->GetActorLocation());
	OwnerCharacter->AttackParticletoActors(TEXT("Attack2"));
}

void UTeam_AIAnimInstance_Boss::AnimNotify_Attack3ParticleCheck()
{
	if (!OwnerCharacter)
		return;
	OwnerCharacter->PlayAudioSystemAtLocation(TEXT("SoundAttack3"), OwnerCharacter->GetActorLocation());
	OwnerCharacter->ActivateParticleSystem(TEXT("Attack3"));
}
