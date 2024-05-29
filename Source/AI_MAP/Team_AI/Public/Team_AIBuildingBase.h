// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI_MAP.h"
#include "GameFramework/Actor.h"
#include "Team_AIBuildingBase.generated.h"

UCLASS()
class AI_MAP_API ATeam_AIBuildingBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeam_AIBuildingBase();

	virtual void PostInitializeComponents() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBoxComponent* CollisionBox;
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//	UStaticMeshComponent* Roof;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UStaticMeshComponent*> Roofs;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	TArray<class UBoxComponent*> CollisionBoxes;
private:
	UFUNCTION()
		void ActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void ActorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
