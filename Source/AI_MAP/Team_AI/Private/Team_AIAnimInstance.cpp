// Fill out your copyright notice in the Description page of Project Settings.


#include "Team_AIAnimInstance.h"
#include "Team_AICharacterBase.h"
UTeam_AIAnimInstance::UTeam_AIAnimInstance()
{
	/*
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FOBJ_AttackMontage(TEXT("'/Game/Level_TeamMap/Blueprint/ANM_Enemy.ANM_Enemy'"));
	if (FOBJ_AttackMontage.Succeeded())
		AttackMontage = FOBJ_AttackMontage.Object;
	*/
	IsDead = false;
}

void UTeam_AIAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	OwnerCharacter = Cast<ATeam_AICharacterBase>(GetOwningActor());
}

void UTeam_AIAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	/*auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
		CurrentPawnSpeed = Cast<ATeam_AICharacterBase>(Pawn)->GetCurrentSpeed();*/
	if(OwnerCharacter)
		CurrentPawnSpeed = OwnerCharacter->GetCurrentSpeed();
}

float UTeam_AIAnimInstance::PlayAttackMontage(float PlayRate, int idx)
{
	/*TArray<FSlotAnimationTrack> slots = AttackMontage->SlotAnimTracks;
	for (const auto& iter : slots)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("%s"), *iter.SlotName.ToString()));
	}*/
	IsAttacking = true;
	return Montage_Play(AttackMontages[idx], PlayRate);
}

float UTeam_AIAnimInstance::PlayDeadMontage(float PlayRate)
{
	IsDead = true;
	return Montage_Play(DeadMontage, PlayRate);
}

float UTeam_AIAnimInstance::PlaySpawnMontage(float PlayRate)
{
	return SpawnMontage ? Montage_Play(SpawnMontage, PlayRate) : -1.0f;
}

bool UTeam_AIAnimInstance::GetAttacking() const
{
	return IsAttacking;
}

bool UTeam_AIAnimInstance::GetDead() const
{
	return IsDead;
}

uint64 UTeam_AIAnimInstance::GetAttackMontagesSize() const
{
	return AttackMontages.Num();
}

void UTeam_AIAnimInstance::AnimNotify_AttackStartCheck()
{
	OnAttackStart.Broadcast();
}

void UTeam_AIAnimInstance::AnimNotify_AttackEndCheck()
{
	IsAttacking = false;
	OnAttackEnd.Broadcast();
}

void UTeam_AIAnimInstance::AnimNotify_AttackMontageEndCheck()
{
	OnAttackMontageEnd.Broadcast();
}

void UTeam_AIAnimInstance::AnimNotify_DeadCheck()
{
	OnDead.Broadcast();
}

void UTeam_AIAnimInstance::AnimNotify_AttackParticleCheck()
{
	OnAttackParticle.Broadcast();
}

void UTeam_AIAnimInstance::AnimNotify_SpawnParticleCheck()
{
	OnSpawnParticle.Broadcast();
}

