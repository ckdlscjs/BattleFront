#pragma once

#include "CoreMinimal.h"
#include "ClientPacketHandler.h"
#include "PacketSession.h"
#include "Protocol.pb.h"
#include "Header_Macro.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NetworkManager.generated.h"

//class AS1Player;

UCLASS()
class AI_MAP_API UNetworkManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void ConnectToGameServer();

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

public:
	// GameServer
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1");
	int16 Port = 7777;

	TSharedPtr<class PacketSession> GameServerSession;

public:
	//UPROPERTY()
	//TMap<uint64, AS1Player*> Players;

	//UPROPERTY()
	//TObjectPtr<AS1Player> MyPlayer;
};


template <typename T> 
void UNetworkManager::SendPacket(T packet) const
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	const SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
	GameServerSession->SendPacket(SendBuffer);
}
