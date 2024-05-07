#include "ClientPacketHandler.h"
#include "BufferReader.h"
#include "NetworkManager.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];


UNetworkManager* GetWorldNetwork(const PacketSessionRef& Session)
{
	for (auto World : GEngine->GetWorldContexts())
	{
		if (const UGameInstance* GameInstance = World.World()->GetGameInstance())
		{
			if (UNetworkManager* NetworkManager = GameInstance->GetSubsystem<UNetworkManager>())
			{
				if (NetworkManager->GameServerSession == Session)
					return NetworkManager;
			}
		}
	}

	return nullptr;
}

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	return false;
}

bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
	//for (auto& Player : pkt.players())
	//{
	//}

	//for (int32 i = 0; i < pkt.players_size(); i++)
	//{
	//	const Protocol::ObjectInfo& Player = pkt.players(i);
	//}

	//// �κ񿡼� ĳ���� �����ؼ� �ε��� ����.
	//Protocol::C_ENTER_GAME EnterGamePkt;
	//EnterGamePkt.set_playerindex(0);

	//if (const UNetworkManager* GameNetwork = GetWorldNetwork(session))
	//{
	//	GameNetwork->SendPacket(EnterGamePkt);
	//}

	return true;
}

bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleSpawn(pkt);
	}

	return true;
}

bool Handle_S_LEAVE_GAME(PacketSessionRef& session, Protocol::S_LEAVE_GAME& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
	}

	return true;
}

bool Handle_S_SPAWN(PacketSessionRef& session, Protocol::S_SPAWN& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleSpawn(pkt);
	}

	return true;
}

bool Handle_S_DESPAWN(PacketSessionRef& session, Protocol::S_DESPAWN& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleDespawn(pkt);
	}

	return true;
}

bool Handle_S_MOVE(PacketSessionRef& session, Protocol::S_MOVE& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleMove(pkt);
	}

	return true;
}

bool Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt)
{
	auto Msg = pkt.msg();

	return true;
}
