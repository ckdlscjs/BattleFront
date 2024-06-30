// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI_MAP.h"
#include "GameFramework/Actor.h"
#include "Team_AIPatrolRoute.generated.h"

UCLASS()
class AI_MAP_API ATeam_AIPatrolRoute : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeam_AIPatrolRoute();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/** Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay */
	virtual void PostInitializeComponents() override;
	UFUNCTION(BlueprintCallable)
	int32 GetSizeOfPatrolRoute() const;
	UFUNCTION(BlueprintCallable)
	FVector GetLocationAtIndex(int32 index) const;
	UFUNCTION(BlueprintCallable)
	bool CheckSpawnPossible() const;
	UFUNCTION(BlueprintCallable)
	const TArray<class ATeam_AISpawnPointPatrol*>& GetSpawnPoints() const;

	class ATeam_AISpawnPointPatrol* GetSpawnPoint(int32 idx);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION(BlueprintCallable)
	void OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USplineComponent* PatrolRoute;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* PatrolBoxCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class ATeam_AISpawnPointPatrol*> SpawnPoints;
public:
};
