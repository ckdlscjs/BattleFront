// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class AI_MAP_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UCharacterAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStatus", Meta = (AllowPrivateAccess = true))
		float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStatus", Meta = (AllowPrivateAccess = true))
		float Angle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStatus", Meta = (AllowPrivateAccess = true))
		bool bFight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStatus", Meta = (AllowPrivateAccess = true))
		bool bDead;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStatus", Meta = (AllowPrivateAccess = true))
		float AimYaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStatus", Meta = (AllowPrivateAccess = true))
		float AimPitch = 0.f;
};
