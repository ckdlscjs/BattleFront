// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "AI_MAP.h"
#include "AIController.h"
#include "Team_AIController.generated.h"

/**
 * 
 */
UCLASS()
class AI_MAP_API ATeam_AIController : public AAIController
{
	GENERATED_BODY()
public:
	ATeam_AIController();
	UFUNCTION(BlueprintCallable)
	virtual void OnAIMoveComplete(FAIRequestID RequestID, EPathFollowingResult::Type Result);
	virtual void PostInitializeComponents() override;
	UFUNCTION(BlueprintCallable)
	void SetState(ECharacterState State);
	UFUNCTION(BlueprintCallable)
	ECharacterState GetState() const;

	virtual void Tick(float DeltaTime) override;
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess();

private:
	UFUNCTION(BlueprintCallable)
	void OnTargetDetect();
	UFUNCTION(BlueprintCallable)
	void OnSensed(const TArray<AActor*>& UpdatedActors);

public:
	static const FName SpawnPosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
	static const FName TargetInRangeKey;
	static const FName StateKey;
	static const FName AttackDelayKey;
	//static const FName AttackAvailableKey;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsServer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBehaviorTree* BT_Asset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBlackboardData* BB_Asset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAISenseConfig_Sight* SenseSight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAISenseConfig_Damage* SenseDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle RepeatTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration_DetectRadius;
private:
};
