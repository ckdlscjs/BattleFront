// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DroneAttackRange.generated.h"

UCLASS()
class AI_MAP_API ADroneAttackRange : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADroneAttackRange();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USphereComponent* SphereComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* StaticMesh;
	UPROPERTY()
		float Duration;
	UPROPERTY()
		float Damage;
};
