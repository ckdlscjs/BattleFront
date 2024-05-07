// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI_MAP.h"
#include "GameFramework/Actor.h"
#include "Team_AIMagneticField.generated.h"
UENUM(BlueprintType)
enum class ESafetyState : uint8
{
	PAUSE UMETA(DisplayName = "Pause"),
	SHRINK UMETA(DisplayName = "Shrink"),
	END UMETA(DisplayName = "End"),
};

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
	PHASEDEFULAT = 0 UMETA(DisplayName = "Time0"),
	PHASE0 = 10 UMETA(DisplayName = "Time1"),
	PHASE1 = 8 UMETA(DisplayName = "Time2"),
	PHASE2 = 6 UMETA(DisplayName = "Time3"),
	PHASE3 = 4 UMETA(DisplayName = "Time4"),
	PHASE4 = 2 UMETA(DisplayName = "Time5"),
};
static TArray<TPair<const EMagneticPhase, const EPhaseTime>> MagenticPhase;
UCLASS()
class AI_MAP_API ATeam_AIMagneticField : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeam_AIMagneticField();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> OverlapActors;

private:
	UFUNCTION(BlueprintCallable, CallInEditor)
		void RandomPoint();
	UFUNCTION(BlueprintCallable, CallInEditor)
		void ShrinkSafetyField(float DeltaTime);
	UFUNCTION()
		void OnMagneticFieldBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnMagneticFieldEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		void OnSafetySphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnSafetySphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		UStaticMeshComponent* MagneticField;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		UStaticMeshComponent* SafetySphere;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		FVector RandPos;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		TSet<AActor*> OverlapedActors;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		int32 CurrentPhase;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		ESafetyState CurrentState;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		float CurrentTime;
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		float */
};
