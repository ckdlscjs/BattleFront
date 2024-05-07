// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once


#include "AI_MAP.h"
#include "GameFramework/GameModeBase.h"
#include "Team_AIGameMode.generated.h"

UCLASS(minimalapi)
class ATeam_AIGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATeam_AIGameMode();

	virtual void StartPlay() override;
	virtual void Tick(float DeltaSeconds) override;
public:
	UFUNCTION(BlueprintCallable)
	void SpawnEnemyRandom();
	UFUNCTION(BlueprintCallable)
	void SpawnEnemyPatrol();
	UFUNCTION(BlueprintCallable)
	int32 GetGameLevel() const;
	UFUNCTION(BlueprintCallable)
	void AddSpawnActor(AActor* actor);
	UFUNCTION(BlueprintCallable)
	void DeleteSpawnActor(AActor* actor);
	UFUNCTION(BlueprintCallable)
	float GetDurationSpawnEnemyRandom() const;
	UFUNCTION(BlueprintCallable)

	float GetDurationSpawnEnemyPatrol() const;
protected:
	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> SpawnPoints_Random;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> PatrolRoutes;

	UPROPERTY(VisibleAnywhere)
	TSet<AActor*> Enemys;

	UPROPERTY(VisibleAnywhere)
	int32 GameLevel;

	UPROPERTY(EditAnywhere)
	float Duration_SpawnEnemyRandom;
	UPROPERTY(EditAnywhere)
	float Duration_SpawnEnemyPatrol;
	UPROPERTY(EditAnywhere)
	float Duration_GameLevel;
	UPROPERTY(EditAnywhere)
	FTimerHandle TimerHandle_GameLevel;
	UPROPERTY(EditAnywhere)
	FTimerHandle TimerHandle_SpawnRandom;
	UPROPERTY(EditAnywhere)
	FTimerHandle TimerHandle_SpawnPatrol;
private:
	
};



