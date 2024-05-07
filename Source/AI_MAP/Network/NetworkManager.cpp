#include "NetworkManager.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "SocketSubsystem.h"
#include "PacketSession.h"
#include "Protocol.pb.h"
#include "ClientPacketHandler.h"
//#include "S1GameInstance.h"
//#include "S1MyPlayer.h"
#include "Kismet/GameplayStatics.h"

void UNetworkManager::ConnectToGameServer()
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress, Ip);

	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connecting To Server...")));

	bool Connected = Socket->Connect(*InternetAddr);

	if (Connected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Success")));

		// Session
		GameServerSession = MakeShared<PacketSession>(Socket);
		GameServerSession->Run();

		{
			Protocol::C_LOGIN Pkt;
			SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(Pkt);
			SendPacket(SendBuffer);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Failed")));
	}
}

void UNetworkManager::DisconnectFromGameServer()
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	Protocol::C_LEAVE_GAME LeavePkt;
	SendPacket(LeavePkt);

	/*if (Socket)
	{
		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get();
		SocketSubsystem->DestroySocket(Socket);
		Socket = nullptr;
	}*/
}

void UNetworkManager::HandleRecvPackets()
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameServerSession->HandleRecvPackets();
}

void UNetworkManager::SendPacket(SendBufferRef SendBuffer)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameServerSession->SendPacket(SendBuffer);
}

void UNetworkManager::HandleSpawn(const Protocol::ObjectInfo& ObjectInfo, bool IsMine)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	//const uint64 ObjectId = ObjectInfo.object_id();
	//if (Players.Find(ObjectId) != nullptr)
	//	return;

	//FVector SpawnLocation(ObjectInfo.pos_info().x(), ObjectInfo.pos_info().y(), ObjectInfo.pos_info().z());

	//if (IsMine)
	//{
	//	auto* PC = UGameplayStatics::GetPlayerController(this, 0);
	//	AS1Player* Player = Cast<AS1Player>(PC->GetPawn());
	//	if (Player == nullptr)
	//		return;

	//	Player->SetPlayerInfo(ObjectInfo.pos_info());
	//	MyPlayer = Player;
	//	Players.Add(ObjectInfo.object_id(), Player);
	//}
	//else
	//{
	//	AS1Player* Player = Cast<AS1Player>(World->SpawnActor(Cast<US1GameInstance>(GetGameInstance())->OtherPlayerClass, &SpawnLocation));
	//	Player->SetPlayerInfo(ObjectInfo.pos_info());
	//	Players.Add(ObjectInfo.object_id(), Player);
	//}
}

void UNetworkManager::HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt)
{
	//HandleSpawn(EnterGamePkt.player(), true);
}

void UNetworkManager::HandleSpawn(const Protocol::S_SPAWN& SpawnPkt)
{
	//for (auto& Player : SpawnPkt.players())
	//{
	//	HandleSpawn(Player, false);
	//}
}

void UNetworkManager::HandleDespawn(uint64 ObjectId)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	//AS1Player** FindActor = Players.Find(ObjectId);
	//if (FindActor == nullptr)
	//	return;

	//World->DestroyActor(*FindActor);
}

void UNetworkManager::HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt)
{
	//for (auto& ObjectId : DespawnPkt.object_ids())
	//{
	//	HandleDespawn(ObjectId);
	//}
}

void UNetworkManager::HandleMove(const Protocol::S_MOVE& MovePkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	//const uint64 ObjectId = MovePkt.info().object_id();
	//AS1Player** FindActor = Players.Find(ObjectId);
	//if (FindActor == nullptr)
	//	return;

	//AS1Player* Player = (*FindActor);
	//if (Player->IsMyPlayer())
	//	return;

	//const Protocol::PosInfo& Info = MovePkt.info();
	////Player->SetPlayerInfo(Info);
	//Player->SetDestInfo(Info);
}