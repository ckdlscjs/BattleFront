// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI_MAP.h"
#include "GameFramework/Actor.h"
#include "Team_AIBushCollision.generated.h"
class UBoxComponent;
class AGameCharacter;
UCLASS()
class AI_MAP_API ATeam_AIBushCollision : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeam_AIBushCollision();
	virtual void PostInitializeComponents();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	UFUNCTION()
	void BushBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void BushEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* CollisionBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool PlayerInBush;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TSet<AActor*> Players;
private:
};
