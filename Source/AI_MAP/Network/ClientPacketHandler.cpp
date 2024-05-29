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
	Protocol::C_ENTER_GAME EnterGamePkt;

	if (const UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->SendPacket(EnterGamePkt);
	}

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

bool Handle_S_FIRE(PacketSessionRef& session, Protocol::S_FIRE& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleAttack(pkt);
	}

	return true;
}

bool Handle_S_HIT(PacketSessionRef& session, Protocol::S_HIT& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleHit(pkt);
	}

	return true;
}

bool Handle_S_AISPAWNRANDOM(PacketSessionRef& session, Protocol::S_AISPAWNRANDOM& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleAISpawn(pkt);
	}

	return true;
}

bool Handle_S_AISPAWNPATROL(PacketSessionRef& session, Protocol::S_AISPAWNPATROL& pkt)
{
	return false;
}

bool Handle_S_AIMOVE(PacketSessionRef& session, Protocol::S_AIMOVE& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleAIMove(pkt);
	}

	return true;
}

bool Handle_S_AIMOVESTOP(PacketSessionRef& session, Protocol::S_AIMOVESTOP& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleAIMoveStop(pkt);
	}

	return true;
}

bool Handle_S_AIATTACK(PacketSessionRef& session, Protocol::S_AIATTACK& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleAIAttack(pkt);
	}
	return true;
}

bool Handle_S_AIROTATE(PacketSessionRef& session, Protocol::S_AIROTATE& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleAIRotate(pkt);
	}
	return true;
}

bool Handle_S_AIHIT(PacketSessionRef& session, Protocol::S_AIHIT& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleAIHit(pkt);
	}
	return true;
}

bool Handle_S_DAMAGED(PacketSessionRef& session, Protocol::S_DAMAGED& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleDamaged(pkt);
	}
	return true;
}

bool Handle_S_AIDAMAGED(PacketSessionRef& session, Protocol::S_AIDAMAGED& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleAIDamaged(pkt);
	}

	return true;
}

bool Handle_S_AIDEAD(PacketSessionRef& session, Protocol::S_AIDEAD& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleAIDead(pkt);
	}
	
	return true;
}

bool Handle_S_PLAYERDEAD(PacketSessionRef& session, Protocol::S_PLAYERDEAD& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleDead(pkt);
	}
	return true;
}

bool Handle_S_AIPROJSPAWN(PacketSessionRef& session, Protocol::S_AIPROJSPAWN& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleAISpawnProjectile(pkt);
	}
	return true;
}
