// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"
#include "GameCharacter.h"
#include "Kismet/KismetMathLibrary.h"
UCharacterAnimInstance::UCharacterAnimInstance()
{
	Speed = 0.f;
	bDead = false;
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		//Speed = Pawn->GetVelocity().Size();
		AGameCharacter* Character = Cast<AGameCharacter>(Pawn);
		Speed = Character->speed;
		bDead = Character->IsDead();
		FVector Vel = Pawn->GetVelocity();
		FVector Dir = UKismetMathLibrary::InverseTransformDirection(Pawn->GetActorTransform(), Vel);
		Angle = UKismetMathLibrary::Conv_VectorToRotator(Dir).Yaw;
		bFight = Character->IsFight();
		AimYaw = Character->GetAimYaw();
	}
}