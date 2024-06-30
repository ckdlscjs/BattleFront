// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Team_AIAnimInstance.h"
#include "Team_AIAnimInstance_Boss.generated.h"
/*
DECLARE_MULTICAST_DELEGATE(FOnAttack1StartCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttack1EndCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttack2StartCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttack2EndCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttack3StartCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttack3EndCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttack4StartCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttack4EndCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttack5StartCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttack5EndCheckDelegate);
*/
/**
 * 
 */
UCLASS()
class AI_MAP_API UTeam_AIAnimInstance_Boss : public UTeam_AIAnimInstance
{
	GENERATED_BODY()
public:
	UTeam_AIAnimInstance_Boss();
	virtual void BeginDestroy() override;
protected:

private:
	UFUNCTION()
	void AnimNotify_Attack1StartCheck();
	UFUNCTION()
	void AnimNotify_Attack1EndCheck();

	UFUNCTION()
	void AnimNotify_Attack2StartCheck();
	UFUNCTION()
	void AnimNotify_Attack2EndCheck();

	UFUNCTION()
	void AnimNotify_Attack3StartCheck();
	UFUNCTION()
	void AnimNotify_Attack3EndCheck();

	UFUNCTION()
	void AnimNotify_Attack4SoundStart();
	UFUNCTION()
	void AnimNotify_Attack4StartCheck();
	UFUNCTION()
	void AnimNotify_Attack4EndCheck();
	UFUNCTION()
	void AnimNotify_Attack2ParticleCheck();
	UFUNCTION()
	void AnimNotify_Attack3ParticleCheck();


	//UFUNCTION()
	//void AnimNotify_Attack5StartCheck();
	//UFUNCTION()
	//void AnimNotify_Attack5EndCheck();




public:
	/*FOnAttack1StartCheckDelegate OnAttack1Start;
	FOnAttack1EndCheckDelegate OnAttack1End;
	FOnAttack2StartCheckDelegate OnAttack2Start;
	FOnAttack2EndCheckDelegate OnAttack2End;
	FOnAttack3StartCheckDelegate OnAttack3Start;
	FOnAttack3EndCheckDelegate OnAttack3End;
	FOnAttack4StartCheckDelegate OnAttack4Start;
	FOnAttack4EndCheckDelegate OnAttack4End;*/
	/*FOnAttack5StartCheckDelegate OnAttack5Start;
	FOnAttack5EndCheckDelegate OnAttack5End;*/
	
protected:
	UPROPERTY(EditAnywhere)
	FTimerHandle BossAttackTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Attack4Delay;
private:
	
};
