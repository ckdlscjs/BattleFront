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
	void Attack();
	void SetRandomIndex();
	void SetNewAbility();
	void AbilityLevelUp(int32 &Index, int32& Level);
	bool FindAbility(int32 Index);
	TArray<class UTexture2D*>& GetRandTextureArray();
	void ClearRandomArray();
	class UTexture2D* GetSelectedTexture();
	int32 GetSelectedIndex();
	void DroneAttack();
	void DroneReturn();
	bool CheckDistanceToDrone();
	void ResetSelectedAbility();
	FVector GetDronePointLocation();
	void HealAbility();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool CheckAbilityLevel(TSubclassOf<class AAbilityBase> Base);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override; 
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TArray<TSubclassOf<class AAbilityBase>> AbilityClassArray;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
		TArray<TSubclassOf<class AAbilityBase>> DefaultAbilityArray;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		USceneComponent* DroneSpawnPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		class USphereComponent* SphereComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TArray<class UTexture2D*> RandTextureArray;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TArray<TSubclassOf<class AAbilityBase>> RandomAbilityArray;
	UPROPERTY()
		TSubclassOf<class AAbilityBase> SelectedAbility;
	UPROPERTY()
		TMap<class AAbilityBase*,int32> AbilityArray;
	UPROPERTY()
		TArray<TSubclassOf<class AAbilityBase>> RangeAbility;
	UPROPERTY()
		class AAbilityBase* DroneActor;
	UPROPERTY()
		class AAbilityBase* HealComponent;
	UPROPERTY()
		FTimerHandle DroneAttackTimer;
	UPROPERTY()
		FTimerHandle HealTimer;
	UPROPERTY()
		bool bCheckDistance;
	UPROPERTY()
		float DroneTimer;
	UPROPERTY()
		float LocalDeltaTime;
public:
	// Server
	class UNetworkManager* GetNetworkManager() const;
	void RecvMakeRange(class AGameCharacter* owner, int abilityIdx, FVector loc);
	void RecvMakeHeal(int abilityIdx);
	void RecvUpdateHP(int hp);
	
	class AAbilityBase* GetDrone();
	void RecvMakeDrone(int64 idx, AGameCharacter* owner);
	void RecvSearchDrone(FVector loc);
	void RecvMoveDrone(FVector loc);
};
