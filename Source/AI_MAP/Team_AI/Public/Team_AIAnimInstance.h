// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI_MAP.h"
#include "Animation/AnimInstance.h"
#include "Team_AIAnimInstance.generated.h"
DECLARE_MULTICAST_DELEGATE(FOnAttackStartCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackEndCheckDelegate);

DECLARE_MULTICAST_DELEGATE(FOnAttackMontageEndCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnDeadCheckDelegate);
/**
 * 
 */

UCLASS()
class AI_MAP_API UTeam_AIAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UTeam_AIAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	float PlayAttackMontage(float PlayRate = 1.0f);
	float PlayDeadMontage(float PlayRate = 1.0f);
	UFUNCTION(BlueprintCallable)
	bool GetAttacking() const;
	UFUNCTION(BlueprintCallable)
	bool GetDead() const;
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

public:
	FOnAttackStartCheckDelegate OnAttackStart;
	FOnAttackEndCheckDelegate OnAttackEnd;
	FOnAttackMontageEndCheckDelegate OnAttackMontageEnd;
	FOnDeadCheckDelegate OnDead;
protected:
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
		UAnimMontage* DeadMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
		bool IsDead;

};
