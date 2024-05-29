// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "AbilityBomb.generated.h"

/**
 * 
 */

UCLASS()
class AI_MAP_API AAbilityBomb : public AAbilityBase
{
	GENERATED_BODY()
public:
	AAbilityBomb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void ProjectileBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void Attack(FVector& Location) override;
	virtual void SetLocation(FVector& Location)override;
	 
};