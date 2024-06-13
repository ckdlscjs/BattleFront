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

	virtual void Logout(AController* Exiting) override;
public:
	UFUNCTION(BlueprintCallable)
	void SpawnEnemyRandom();
	UFUNCTION(BlueprintCallable)
	void SpawnEnemyPatrol();

	UFUNCTION(BlueprintCallable)
	void SpawnEnemyBoss();
	UFUNCTION(BlueprintCallable)
	int32 GetGameLevel() const;
	void AddSpawnActor(const Protocol::S_AISPAWN_RANDOM& AiSpawnPkt);
	void AddSpawnActor(const Protocol::S_AISPAWN_PATROL& AiSpawnPkt);
	void AddSpawnActor(const Protocol::S_AISPAWN_BOSS& AiSpawnPkt);

	UFUNCTION(BlueprintCallable)
	float GetDurationSpawnEnemyRandom() const;
	UFUNCTION(BlueprintCallable)
	float GetDurationSpawnEnemyPatrol() const;

	//--------ServerFunctions----------
	//Players
	TMap<uint64, AGameCharacter*>& GetPlayers();
	void SetPlayerInfo(bool isMine, const Protocol::PosInfo& Info);
	const TObjectPtr<AGameCharacter>& GetMyPlayer() const;
	void SetPlayerMove(const Protocol::S_MOVE& MovePkt);
	void SetPlayerAttack(const Protocol::S_FIRE& FirePkt);
	void SetPlayerDamaged(const Protocol::S_DAMAGED& dmgPkt);
	void SetPlayerDead(const Protocol::S_PLAYERDEAD& playerDeadPkt);
	void SetPlayerSKill(const Protocol::S_PLAYERSKILL_RANGE& rangePkt);
	void SetPlayerSKill(const Protocol::S_PLAYERSKILL_GUARD& guardPkt);
	void SetPlayerSKill(const Protocol::S_PLAYERSKILL_HEAL& healPkt);
	void SetUpdatedHeal(const Protocol::S_PLAYERHEAL& healPkt);
	void SetMakeDrone(const Protocol::S_MAKEDRONE& makeDrnPkt);
	void SetSearchDrone(const Protocol::S_SEARCHDRONE& searchDrnPkt);
	void SetMoveDrone(const Protocol::S_MOVEDRONE& moveDrnPkt);

	void SetPlayerDespawn(uint64 objectID);
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
	void SetKnocksBack(const Protocol::S_AI_KNOCKS_BACK& AIKnocksBackPkt);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> SpawnPoints_Player;
	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> SpawnPoints_Random;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> PatrolRoutes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> SpawnPoints_Boss;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ATeam_AIMagneticField* MagneticField;


	UPROPERTY(VisibleAnywhere)
	int32 GameLevel;

	UPROPERTY(EditAnywhere)
	float Duration_SpawnEnemyRandom;
	UPROPERTY(EditAnywhere)
	float Duration_SpawnEnemyPatrol;
	UPROPERTY(EditAnywhere)
	float Duration_SpawnEnemyBoss;
	UPROPERTY(EditAnywhere)
	float Duration_GameLevel;
	UPROPERTY(EditAnywhere)
	FTimerHandle TimerHandle_GameLevel;
	UPROPERTY(EditAnywhere)
	FTimerHandle TimerHandle_SpawnRandom;
	UPROPERTY(EditAnywhere)
	FTimerHandle TimerHandle_SpawnPatrol;

	UPROPERTY(EditAnywhere)
	FTimerHandle TimerHandle_SpawnBoss;
	

	UPROPERTY()
	TMap<uint64, AGameCharacter*> Players;

	UPROPERTY()
	TObjectPtr<AGameCharacter> MyPlayer;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AGameCharacter> PlayerClass;

	UPROPERTY()
	TMap<uint64, ATeam_AICharacterBase*> Enemies;
	UPROPERTY()
	ATeam_AICharacterBase* Boss;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AProjectile>> ClassesProjectiles;
	UPROPERTY()
	TMap<uint64, AProjectile*> Projectiles_Players;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ATeam_AIProjectileBase>> AIClassesProjectiles;
	UPROPERTY()
	TMap<uint64, ATeam_AIProjectileBase*> Projectiles_Enemies;

// Server
public:
	UNetworkManager* GetNetworkManager() const;
	void InitializeStartPlay();
	TArray<TSubclassOf<ATeam_AIProjectileBase>>& GetAIProjClasses();
	int64 WinnerCheck();

	ATeam_AICharacterBase* GetBoss();

	//virtual void Logout(AController* Exiting) override;

};



