// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once


#include "AI_MAP.h"
#include "GameFramework/GameModeBase.h"
#include "NetworkManager.h"
#include "Team_AIGameMode.generated.h"

class AGameCharacter;
class ATeam_AICharacterBase;
class ATeam_AIProjectileBase;
class AProjectile;
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

	void AddSpawnActor(const Protocol::ObjectInfo& AiInfo);
	void AddSpawnActor(const Protocol::S_AISPAWNRANDOM& AiRandomSpawnPkt);

	void DeleteSpawnActor(const Protocol::ObjectInfo& AiInfo);
	UFUNCTION(BlueprintCallable)
	float GetDurationSpawnEnemyRandom() const;
	UFUNCTION(BlueprintCallable)
	float GetDurationSpawnEnemyPatrol() const;

	class AAbilityManager* GetAbilityManager();
	//--------ServerFunctions----------
	//Players
	const TMap<uint64, AGameCharacter*>& GetPlayers() const;
	void SetPlayerInfo(bool isMine, const Protocol::PosInfo& Info);
	const TObjectPtr<AGameCharacter>& GetMyPlayer() const;
	void SetPlayerMove(const Protocol::S_MOVE& MovePkt);
	void SetPlayerAttack(const Protocol::S_FIRE& FirePkt);
	void SetPlayerDamaged(const Protocol::S_DAMAGED& dmgPkt);
	void SetPlayerDead(const Protocol::S_PLAYERDEAD& playerDeadPkt);

	//Enemies
	const TMap<uint64, ATeam_AICharacterBase*>& GetEnemies() const;
	void SetAIMove(const Protocol::S_AIMOVE& AIMovePkt);
	void SetAIMoveStop(const Protocol::S_AIMOVESTOP& AIMoveStopPkt);
	void SetAIAttack(const Protocol::S_AIATTACK& AIattackPkt);
	void SetAIRotate(const Protocol::S_AIROTATE& AIRotPkt);
	void SetAIHit(const Protocol::S_AIHIT& AIHitPkt);
	void SetAISpawnProj(const Protocol::S_AIPROJSPAWN& AIProjPkt);
	void SetAIDamaged(const Protocol::S_AIDAMAGED& AIDmgedPkt);
	void SetAIDead(const Protocol::S_AIDEAD& AIDeadPkt);
	void SetAIDespawn(uint64 objectID);
protected:
	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> SpawnPoints_Random;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> PatrolRoutes;

	//UPROPERTY(VisibleAnywhere)
	//TSet<AActor*> Enemies;

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
	

	UPROPERTY()
	TMap<uint64, AGameCharacter*> Players;

	UPROPERTY()
	TObjectPtr<AGameCharacter> MyPlayer;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AGameCharacter> PlayerClass;

	//UPROPERTY()
	TMap<uint64, ATeam_AICharacterBase*> Enemies;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ATeam_AICharacterBase> AIClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ATeam_AICharacterBase> AIClassRecv;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ATeam_AICharacterBase>> AIClassesSend;
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ATeam_AICharacterBase>> AIClassesRecv;


	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AProjectile>> ClassesProjectiles;
	UPROPERTY()
	TMap<uint64, AProjectile*> Projectiles_Players;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ATeam_AIProjectileBase>> AIClassesProjectiles;
	UPROPERTY()
	TMap<uint64, ATeam_AIProjectileBase*> Projectiles_Enemies;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Manager", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AAbilityManager> AbilityManagerClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class AAbilityManager* AbilityManager;


	void SpawnAbilityManager();
// Server
public:
	UNetworkManager* GetNetworkManager() const;
	void InitializeStartPlay();
	TArray<TSubclassOf<ATeam_AIProjectileBase>>& GetAIProjClasses();
	int64 WinnerCheck();
};



