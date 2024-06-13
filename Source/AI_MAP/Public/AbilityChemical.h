// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "AbilityChemical.generated.h"

/**
 * 
 */
UCLASS()
class AI_MAP_API AAbilityChemical : public AAbilityBase
{
	GENERATED_BODY()
public:
	AAbilityChemical();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetLocation(FVector& Location) override;
	virtual void AbilityLevelUp() override;
	virtual int32 GetProjCount() override;
	void TakePlayerDamage();
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USphereComponent* SphereCollision;
	UPROPERTY()
		FTimerHandle DamageTimerHandle;
	UPROPERTY()
		float ChemicalDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
		float Duration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UParticleSystemComponent* ChemicalParticle;
};
