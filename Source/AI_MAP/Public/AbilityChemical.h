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
	virtual void ProjectileBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetLocation(FVector& Location) override;
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
