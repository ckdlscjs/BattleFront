// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI_MAP.h"
#include "Animation/AnimInstance.h"
#include "Team_AIAnimInstance.generated.h"
DECLARE_MULTICAST_DELEGATE(FOnAttackStartCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackEndCheckDelegate);

DECLARE_MULTICAST_DELEGATE(FOnAttackMontageEndCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnDeadCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackParticleCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnSpawnParticleCheckDelegate);
/**
 * 
 */
class ATeam_AICharacterBase;
UCLASS()
class AI_MAP_API UTeam_AIAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UTeam_AIAnimInstance();

	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	float PlayAttackMontage(float PlayRate = 1.0, int idx = 0);
	float PlayDeadMontage(float PlayRate = 1.0f);

	float PlaySpawnMontage(float PlayRate = 1.0f);
	UFUNCTION(BlueprintCallable)
	bool GetAttacking() const;
	UFUNCTION(BlueprintCallable)
	bool GetDead() const;
	
	uint64 GetAttackMontagesSize() const;
protected:

private:
	UFUNCTION()
	void AnimNotify_AttackStartCheck();
	UFUNCTION()
	void AnimNotify_AttackEndCheck();
	UFUNCTION()
	void AnimNotify_AttackMontageEndCheck();
	UFUNCTION()
	void AnimNotify_DeadCheck();

	UFUNCTION()
	void AnimNotify_AttackParticleCheck();

	UFUNCTION()
	void AnimNotify_SpawnParticleCheck();

public:
	FOnAttackStartCheckDelegate OnAttackStart;
	FOnAttackEndCheckDelegate OnAttackEnd;
	FOnAttackMontageEndCheckDelegate OnAttackMontageEnd;
	FOnDeadCheckDelegate OnDead;
	FOnAttackParticleCheckDelegate OnAttackParticle;
	FOnSpawnParticleCheckDelegate OnSpawnParticle;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentPawnSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> AttackMontages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* DeadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* SpawnMontage;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> HitMotages;*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAttacking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDead;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATeam_AICharacterBase* OwnerCharacter;
private:
	
};
