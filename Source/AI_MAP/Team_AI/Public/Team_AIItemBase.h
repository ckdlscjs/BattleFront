// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI_Map.h"
#include "GameFramework/Actor.h"
#include "Team_AIItemBase.generated.h"
class UBoxComponent;
class AGameCharacter;
class UParticleSystemComponent;
class UMaterialInstanceDynamic;
UCLASS()
class AI_MAP_API ATeam_AIItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeam_AIItemBase();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	UFUNCTION(BlueprintImplementableEvent)
	void GetItemAbility(AGameCharacter* gameCharacter, double value);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:

	UFUNCTION()
	void ItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	


public:
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double ItemAbilityInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* ParticleSystem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstanceDynamic* MaterialDynamicInst;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ItemValue;
	float MeshDissolveTime;
	float DissolveTimeCurrent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle ItemTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float rotTime2Pi;
	float rotTimeCur;
private:
};
