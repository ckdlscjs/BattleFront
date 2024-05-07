// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "AbilityManager.generated.h"

UCLASS()
class AI_MAP_API AAbilityManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbilityManager();
	void Attack(FVector& Loc);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class AAbilityBase* Ability;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<class AAbilityBase*> AbilityArray;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TSubclassOf<class AAbilityBase> AbilityClass;
};
