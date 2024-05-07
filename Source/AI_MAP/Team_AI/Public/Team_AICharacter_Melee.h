// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Team_AICharacterBase.h"
#include "Team_AICharacter_Melee.generated.h"

/**
 * 
 */
UCLASS()
class AI_MAP_API ATeam_AICharacter_Melee : public ATeam_AICharacterBase
{
	GENERATED_BODY()
public:
	ATeam_AICharacter_Melee();
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
	// Called PostInitialize
	//virtual void PostInitializeComponents() override;
	// Called to bind functionality to input
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	// Called to bind functionality to input
protected:

private:

public:
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* AttackCollision;
private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
