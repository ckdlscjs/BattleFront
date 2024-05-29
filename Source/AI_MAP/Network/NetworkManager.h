#pragma once

#include "CoreMinimal.h"
#include "ClientPacketHandler.h"
#include "PacketSession.h"
#include "Protocol.pb.h"
#include "Header_Macro.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NetworkManager.generated.h"


class ATeam_AIGameMode;
UCLASS()
class AI_MAP_API UNetworkManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UNetworkManager();

	UFUNCTION(BlueprintCallable)
	void ConnectToGameServer();

	UFUNCTION(BlueprintCallable)

	bool SetGameMode(ATeam_AIGameMode* worldGameMode);

	UFUNCTION(BlueprintCallable)
	void DisconnectFromGameServer();

	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

	void SendPacket(SendBufferRef SendBuffer);

	template<typename T>
	void SendPacket(T packet) const;

public:
	void HandleSpawn(const Protocol::ObjectInfo& PlayerInfo, bool IsMine);
	void HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt);
	void HandleSpawn(const Protocol::S_SPAWN& SpawnPkt);

	void HandleDespawn(uint64 ObjectId);
	void HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt);

	void HandleMove(const Protocol::S_MOVE& MovePkt);

	void HandleAttack(const Protocol::S_FIRE& FirePkt);
	void HandleHit(const Protocol::S_HIT& HitPkt);
	void HandleDamaged(const Protocol::S_DAMAGED& dmgPkt);
	void HandleDead(const Protocol::S_PLAYERDEAD& playerDeadPkt);

	// AI
	void HandleAISpawn(const Protocol::ObjectInfo& AiInfo);
	void HandleAISpawn(const Protocol::S_AISPAWNRANDOM& AiRandomSpawnPkt);
	void HandleAISpawn(const Protocol::S_AISPAWNPATROL& AiPatrolSpawnPkt);

	void HandleAIMove(const Protocol::S_AIMOVE& AIMovePkt);
	void HandleAIMoveStop(const Protocol::S_AIMOVESTOP& AIMoveStopPkt);
	void HandleAIAttack(const Protocol::S_AIATTACK& AIattackPkt);
	void HandleAIRotate(const Protocol::S_AIROTATE& AIRotPkt);
	void HandleAIHit(const Protocol::S_AIHIT& AIHitPkt);
	void HandleAISpawnProjectile(const Protocol::S_AIPROJSPAWN& AIProjPkt);
	void HandleAIDamaged(const Protocol::S_AIDAMAGED& AIDmgedPkt);
	void HandleAIDead(const Protocol::S_AIDEAD& AIDeadPkt);
public:
	// GameServer
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1");
	int16 Port = 7777;

	TSharedPtr<class PacketSession> GameServerSession;

public:
	UPROPERTY()
	TObjectPtr<ATeam_AIGameMode> GameMode;
	/*UPROPERTY()
	TMap<uint64, AGameCharacter*> Players;

	UPROPERTY()
	TObjectPtr<AGameCharacter> MyPlayer;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AGameCharacter> PlayerClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ATeam_AICharacterBase> AIClass;

	UPROPERTY(EditAnywhere)
	TMap<uint64, TSubclassOf<ATeam_AICharacterBase>> AIClassesRecv;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ATeam_AICharacterBase> AIClassRecv;

	UPROPERTY()
	TMap<uint64, ATeam_AICharacterBase*> Enemies;*/
};


template <typename T> 
void UNetworkManager::SendPacket(T packet) const
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	const SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
	GameServerSession->SendPacket(SendBuffer);
}
