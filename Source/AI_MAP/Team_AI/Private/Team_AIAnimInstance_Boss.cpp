// Fill out your copyright notice in the Description page of Project Settings.


#include "Team_AIAnimInstance_Boss.h"

void UTeam_AIAnimInstance_Boss::AnimNotify_Attack1StartCheck()
{
	OnAttack1Start.Broadcast();
}

void UTeam_AIAnimInstance_Boss::AnimNotify_Attack1EndCheck()
{
	IsAttacking = false;
	OnAttack1End.Broadcast();
}
void UTeam_AIAnimInstance_Boss::AnimNotify_Attack2StartCheck()
{
	OnAttack2Start.Broadcast();
}

void UTeam_AIAnimInstance_Boss::AnimNotify_Attack2EndCheck()
{
	IsAttacking = false;
	OnAttack2End.Broadcast();
}

void UTeam_AIAnimInstance_Boss::AnimNotify_Attack3StartCheck()
{
	OnAttack3Start.Broadcast();
}

void UTeam_AIAnimInstance_Boss::AnimNotify_Attack3EndCheck()
{
	IsAttacking = false;
	OnAttack3End.Broadcast();
}

void UTeam_AIAnimInstance_Boss::AnimNotify_Attack4StartCheck()
{
	OnAttack4Start.Broadcast();
}

void UTeam_AIAnimInstance_Boss::AnimNotify_Attack4EndCheck()
{
	IsAttacking = false;
	OnAttack4End.Broadcast();
}

void UTeam_AIAnimInstance_Boss::AnimNotify_Attack2ParticleCheck()
{
	OnAttack2Particle.Broadcast();
}

void UTeam_AIAnimInstance_Boss::AnimNotify_Attack3ParticleCheck()
{
	OnAttack3Particle.Broadcast();
}

//void UTeam_AIAnimInstance_Boss::AnimNotify_Attack5StartCheck()
//{
//	OnAttack5Start.Broadcast();
//}

//void UTeam_AIAnimInstance_Boss::AnimNotify_Attack5EndCheck()
//{
//	IsAttacking = false;
//	OnAttack5End.Broadcast();
//}