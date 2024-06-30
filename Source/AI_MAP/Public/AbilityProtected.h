// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "AbilityProtected.generated.h"

/**
 * 
 */
UCLASS()
class AI_MAP_API AAbilityProtected : public AAbilityBase
{
	GENERATED_BODY()
	
public:
	AAbilityProtected();

protected:
	virtual void BeginPlay()override;
public:
	virtual void Tick(float DeltaTime) override;
public:
	virtual float GetAbilityDetail() override;
	virtual void ChargeGuardPoint() override;
	virtual void AbilityLevelUp() override;
	virtual void SetAbilityDetail(float Details) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
		float GuardPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
		float AbilityPoint;

};
