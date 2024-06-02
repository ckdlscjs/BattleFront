// Fill out your copyright notice in the Description page of Project Settings.


#include "Team_AICharacter_Recv.h"
#include "Team_AIAnimInstance.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/AnimBlueprint.h"
#include "Particles/ParticleSystemComponent.h"

ATeam_AICharacter_Recv::ATeam_AICharacter_Recv()
{
}

void ATeam_AICharacter_Recv::BeginPlay()
{
	Super::BeginPlay();
}

void ATeam_AICharacter_Recv::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	/*AIAttack.BindLambda([this]() -> void
		{
			BehaviorAttack();
		});
	AIDead.BindLambda([this]() -> void
		{
			BehaviorDead();
		});
	AIMove.BindLambda([this](FVector location, FRotator rotate, float movespeed) -> void
		{
			SetActorLocation(location);
			SetActorRotation(rotate);
			SpeedCurrent = movespeed;
		});
	/*
	AnimInstance->OnAttackParticle.AddLambda([this]() -> void
		{
			if (AttackParticleSystem->Template)
				AttackParticleSystem->Activate(true);
		});
	*/

}

//void ATeam_AICharacter_Recv::RecvTest(FString str)
//{
//	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("%s"), *str), true, true, FColor::Red);
//	if (str == "MoveTo")
//		AIMove.Execute(GetActorLocation(), GetActorRotation(), 100);
//	else if (str == "Attack")
//		AIAttack.Execute();
//	else if (str == "Dead")
//		AIDead.Execute();
//	else
//		SpeedCurrent = 0;
//}

bool ATeam_AICharacter_Recv::RecvMoveTo(FVector loc, FRotator rot, float speed)
{
	SetActorLocation(loc);
	SetActorRotation(rot);
	SpeedCurrent = speed;
	return true;
}

bool ATeam_AICharacter_Recv::RecvAttack(int32 idx)
{
	BehaviorAttack(idx);
	return true;
}
/*
bool ATeam_AICharacter_Recv::RecvAttackParticle()
{
	//recv activateParticle
	//ActiveAttackParticle();
	return true;
}
*/
bool ATeam_AICharacter_Recv::RecvRotate(FRotator rot)
{
	SetActorRotation(rot);
	return true;
}

bool ATeam_AICharacter_Recv::RecvDead()
{
	BehaviorDead();
	return true;
}
