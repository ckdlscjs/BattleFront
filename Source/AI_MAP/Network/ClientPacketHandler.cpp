#include "ClientPacketHandler.h"
#include "BufferReader.h"
#include "NetworkManager.h"
#include "Team_AIGameMode.h"


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
	UNetworkManager* GameNetwork = GetWorldNetwork(session);
	GameNetwork->ClientID = pkt.client_id();

	if (GameNetwork->ClientID == 1)
	{
		Protocol::C_PLAYERSPAWNPOINT spawnPkt;
		spawnPkt.set_spawnpointsize(GameNetwork->GameMode->GetPlayerSpawnPointSize());
		GameNetwork->SendPacket(spawnPkt);
	}

	Protocol::C_PLAYERCOUNT countPkt;
	GameNetwork->SendPacket(countPkt);

	//캐릭터 생성을 호스트가 gamestart trigger 누르면 실행되게 하는 방향으로
	//Protocol::C_ENTER_GAME EnterGamePkt;

	//GameNetwork->SendPacket(EnterGamePkt);

	return true;
}

bool Handle_S_PLAYERCOUNT(PacketSessionRef& session, Protocol::S_PLAYERCOUNT& pkt)
{
	UNetworkManager* GameNetwork = GetWorldNetwork(session);
	GameNetwork->HandlePlayerCount(pkt);
	return true;
}

bool Handle_S_GAMESTART(PacketSessionRef& session, Protocol::S_GAMESTART& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		Protocol::C_ENTER_GAME EnterGamePkt;
		EnterGamePkt.set_client_id(GameNetwork->ClientID);
		GameNetwork->SendPacket(EnterGamePkt);

		GameNetwork->HandleRmvStartWidget(pkt);
		GameNetwork->ChangeMainBGM(pkt);
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
		GameNetwork->ExitGame();
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

bool Handle_S_EXPUP(PacketSessionRef& session, Protocol::S_EXPUP& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleExpUP(pkt);
	}

	return true;
}

bool Handle_S_LVUP(PacketSessionRef& session, Protocol::S_LVUP& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleLvUP(pkt);
	}
	return true;
}

bool Handle_S_AISPAWN_RANDOM(PacketSessionRef& session, Protocol::S_AISPAWN_RANDOM& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleAISpawn(pkt);
	}
	return true;
}

bool Handle_S_AISPAWN_PATROL(PacketSessionRef& session, Protocol::S_AISPAWN_PATROL& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleAISpawn(pkt);
	}
	return true;
}

bool Handle_S_AISPAWN_BOSS(PacketSessionRef& session, Protocol::S_AISPAWN_BOSS& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleAISpawn(pkt);
	}
	return true;
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

bool Handle_S_AIATTACK_BOSS2(PacketSessionRef& session, Protocol::S_AIATTACK_BOSS2& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleAIAttackBoss(pkt);
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

bool Handle_S_AI_KNOCKS_BACK(PacketSessionRef& session, Protocol::S_AI_KNOCKS_BACK& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleAIKnocksBack(pkt);
	}
	return true;
}

bool Handle_S_PLAYERSKILL_BOMB(PacketSessionRef& session, Protocol::S_PLAYERSKILL_BOMB& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleSkillBomb(pkt);
	}
	return true;
}

bool Handle_S_PLAYERSKILL_GRANADE(PacketSessionRef& session, Protocol::S_PLAYERSKILL_GRANADE& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleSkillGranade(pkt);
	}
	return true;
}

bool Handle_S_PLAYERSKILL_CHEMICAL(PacketSessionRef& session, Protocol::S_PLAYERSKILL_CHEMICAL& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleSkillChemical(pkt);
	}
	return false;
}

bool Handle_S_PLAYERSKILL_GUARD(PacketSessionRef& session, Protocol::S_PLAYERSKILL_GUARD& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleSkillGuard(pkt);
	}

	return true;
}

bool Handle_S_PLAYERSKILL_HEAL(PacketSessionRef& session, Protocol::S_PLAYERSKILL_HEAL& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleSkillHeal(pkt);
	}
	return true;
}

bool Handle_S_PLAYERHEAL(PacketSessionRef& session, Protocol::S_PLAYERHEAL& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleHealed(pkt);
	}
	return true;
}

bool Handle_S_MAKEDRONE(PacketSessionRef& session, Protocol::S_MAKEDRONE& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleMakeDrone(pkt);
	}
	return true;
}

bool Handle_S_MOVEDRONE(PacketSessionRef& session, Protocol::S_MOVEDRONE& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleMoveDrone(pkt);
	}
	return false;
}

bool Handle_S_SEARCHDRONE(PacketSessionRef& session, Protocol::S_SEARCHDRONE& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleSearchDrone(pkt);
	}
	return true;
}

bool Handle_S_RETURNDRONE(PacketSessionRef& session, Protocol::S_RETURNDRONE& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleReturnDrone(pkt);
	}
	return true;
}

bool Handle_S_ATTACKDRONE(PacketSessionRef& session, Protocol::S_ATTACKDRONE& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleAttackDrone(pkt);
	}
	return true;
}

bool Handle_S_EATITEM_MAXHPUP(PacketSessionRef& session, Protocol::S_EATITEM_MAXHPUP& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleEatItemMaxHP(pkt);
	}
	return true;
}

bool Handle_S_EATITEM_LVUP(PacketSessionRef& session, Protocol::S_EATITEM_LVUP& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleEatItemLVUP(pkt);
	}
	return true;
}

bool Handle_S_EATITEM_DMGUP(PacketSessionRef& session, Protocol::S_EATITEM_DMGUP& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleEatItemDmgUP(pkt);
	}
	return true;
}

bool Handle_S_EATITEM_HEALHP(PacketSessionRef& session, Protocol::S_EATITEM_HEALHP& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleEatItemHealUp(pkt);
	}
	return true;
}

bool Handle_S_SET_MAGNETICFIELD(PacketSessionRef& session, Protocol::S_SET_MAGNETICFIELD& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleSetMagneticField(pkt);
	}
	return true;
}

bool Handle_S_WORLD_LVUP(PacketSessionRef& session, Protocol::S_WORLD_LVUP& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleSetWorldLV(pkt);
	}
	return true;
}

bool Handle_S_GAMERESULT(PacketSessionRef& session, Protocol::S_GAMERESULT& pkt)
{
	if (UNetworkManager* GameNetwork = GetWorldNetwork(session))
	{
		GameNetwork->HandleGameResult(pkt);
	}
	return true;
}
