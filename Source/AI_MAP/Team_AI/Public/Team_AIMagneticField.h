// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI_MAP.h"
#include "GameFramework/Actor.h"
#include "Team_AIMagneticField.generated.h"
/*
UENUM(BlueprintType)
enum class ESafetyState : uint8
{
	PAUSE UMETA(DisplayName = "Pause"),
	SHRINK UMETA(DisplayName = "Shrink"),
	END UMETA(DisplayName = "End"),
};
*/
UENUM(BlueprintType)
enum class EMagneticPhase: uint8
{
	Default = 0,
	PHASEDEFULAT = 100 UMETA(DisplayName = "Phase0"),
	PHASE0 = 70 UMETA(DisplayName = "Phase1"),
	PHASE1 = 50 UMETA(DisplayName = "Phase2"),
	PHASE2 = 30 UMETA(DisplayName = "Phase3"),
	PHASE3 = 15 UMETA(DisplayName = "Phase4"),
	PHASE4 = 0 UMETA(DisplayName = "Phase5"),
};

UENUM(BlueprintType)
enum class EPhaseTime : uint8
{
	Default = 0,
	PHASEDEFULAT = 60 UMETA(DisplayName = "Time0"),
	PHASE0 = 60 UMETA(DisplayName = "Time1"),
	PHASE1 = 40 UMETA(DisplayName = "Time2"),
	PHASE2 = 30 UMETA(DisplayName = "Time3"),
	PHASE3 = 20 UMETA(DisplayName = "Time4"),
	PHASE4 = 10 UMETA(DisplayName = "Time5"),
};

UCLASS()
class AI_MAP_API ATeam_AIMagneticField : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeam_AIMagneticField();
	// Called every frame
	virtual void PostInitializeComponents() override;
	virtual void BeginDestroy() override;
	float GetRemainTime() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	UFUNCTION(BlueprintCallable, CallInEditor)
	void RandomPoint();
	UFUNCTION(BlueprintCallable, CallInEditor)
	void ShrinkSafetyField();
	UFUNCTION(BlueprintCallable, CallInEditor)
	void PauseSafetyField();
	void UpdateSafetyFieldValue();
public:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MagneticField;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* SafetySphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector RandPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSet<AActor*> OverlapedActors;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentPhase;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//ESafetyState CurrentState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle PauseTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PauseTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle MagenticTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle DamageTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector CenterCur;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector CenterNext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialParameterCollection* FMC_SafetyField;

	TArray<TPair<const EMagneticPhase, const EPhaseTime>> MagenticPhase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float retTime;

public:
	void StartMagnaticField();
	void RecvUpdateSafetyFieldValue(FLinearColor rgb, float radius, float time);
	class UNetworkManager* GetNetworkManager() const;
};
