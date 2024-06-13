// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "AbilityHeal.generated.h"

/**
 * 
 */
UCLASS()
class AI_MAP_API AAbilityHeal : public AAbilityBase
{
	GENERATED_BODY()
	
public:
	AAbilityHeal();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual	void HealCharacterHp(float& HealthPoint, float MaxPoint) override;
	virtual void AbilityLevelUp() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
		float HealAmount;
};
