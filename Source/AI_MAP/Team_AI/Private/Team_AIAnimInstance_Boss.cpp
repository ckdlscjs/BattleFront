// Fill out your copyright notice in the Description page of Project Settings.


#include "Team_AIAnimInstance_Boss.h"
#include "Team_AICharacter_Boss.h"
#include "Particles/ParticleSystemComponent.h"

UTeam_AIAnimInstance_Boss::UTeam_AIAnimInstance_Boss()
{
	Attack4Delay = 1.0f;
}

void UTeam_AIAnimInstance_Boss::AnimNotify_Attack1StartCheck()
{
	IsAttacking = true;
	auto Boss = Cast<ATeam_AICharacter_Boss>(OwnerCharacter);
	if (!Boss)
		return;
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

void UTeam_AIAnimInstance_Boss::AnimNotify_Attack4StartCheck()
{
	IsAttacking = true;
	GetWorld()->GetTimerManager().SetTimer
	(
		BossAttackTimerHandle,
		[this]() -> void
		{
			
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
	OwnerCharacter->AttackParticletoActors(TEXT("Attack2"));
}

void UTeam_AIAnimInstance_Boss::AnimNotify_Attack3ParticleCheck()
{
	if (!OwnerCharacter)
		return;
	OwnerCharacter->ActivateParticleSystem(TEXT("Attack3"));
}
