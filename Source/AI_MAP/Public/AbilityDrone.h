// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "AbilityDrone.generated.h"

/**
 * 
 */
UCLASS()
class AI_MAP_API AAbilityDrone : public AAbilityBase
{
	GENERATED_BODY()
public:
	AAbilityDrone();

protected:
	virtual void BeginPlay()override;
	
public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetLocation(FVector& Location) override;
	void FindEnemy(TArray<AActor*> Actors);
	bool MoveToTarget();
	void ChangeAttackStatus(bool bChange);
	bool GetAttackState();
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USceneComponent* ProjectileSpawnPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UCharacterMovementComponent* MoveComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USphereComponent* SphereComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class ADroneAttackRange> DroneAttackClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
		float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
		FVector TargetLocation;
	UPROPERTY()
		float TargetDistance;
	UPROPERTY()
		float MY_DeltaTime;
	UPROPERTY()
		bool bAttack;
	UPROPERTY()
		FVector Direction;
	UPROPERTY()
		float CurrentDistance = 0.f;
	UPROPERTY()
		FVector StartLocation;
};