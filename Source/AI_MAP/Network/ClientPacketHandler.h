#pragma once
#include "Protocol.pb.h"

#if UE_BUILD_DEBUG + UE_BUILD_DEVELOPMENT + UE_BUILD_TEST + UE_BUILD_SHIPPING >= 1
#include "Header_Macro.h"
#endif

using PacketHandlerFunc = std::function<bool(PacketSessionRef&, BYTE*, int32)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

// Utils Func
class UNetworkManager* GetWorldNetwork(const PacketSessionRef& Session);

enum : uint16
{
	PKT_C_LOGIN = 1000,
	PKT_S_LOGIN = 1001,
	PKT_C_ENTER_GAME = 1002,
	PKT_S_ENTER_GAME = 1003,
	PKT_C_LEAVE_GAME = 1004,
	PKT_S_LEAVE_GAME = 1005,
	PKT_S_SPAWN = 1006,
	PKT_S_DESPAWN = 1007,
	PKT_C_MOVE = 1008,
	PKT_S_MOVE = 1009,
	PKT_C_CHAT = 1010,
	PKT_S_CHAT = 1011,
	PKT_C_FIRE = 1012,
	PKT_S_FIRE = 1013,
	PKT_C_HIT = 1014,
	PKT_S_HIT = 1015,
	PKT_C_AISPAWN_RANDOM = 1016,
	PKT_S_AISPAWN_RANDOM = 1017,
	PKT_C_AISPAWN_PATROL = 1018,
	PKT_S_AISPAWN_PATROL = 1019,
	PKT_C_AISPAWN_BOSS = 1020,
	PKT_S_AISPAWN_BOSS = 1021,
	PKT_S_AIMOVE = 1022,
	PKT_C_AIMOVE = 1023,
	PKT_S_AIMOVESTOP = 1024,
	PKT_C_AIMOVESTOP = 1025,
	PKT_S_AIATTACK = 1026,
	PKT_C_AIATTACK = 1027,
	PKT_S_AIROTATE = 1028,
	PKT_C_AIROTATE = 1029,
	PKT_S_AIHIT = 1030,
	PKT_C_AIHIT = 1031,
	PKT_S_DAMAGED = 1032,
	PKT_C_DAMAGED = 1033,
	PKT_S_AIDAMAGED = 1034,
	PKT_C_AIDAMAGED = 1035,
	PKT_S_AIDEAD = 1036,
	PKT_C_AIDEAD = 1037,
	PKT_S_PLAYERDEAD = 1038,
	PKT_C_PLAYERDEAD = 1039,
	PKT_S_AIPROJSPAWN = 1040,
	PKT_C_AIPROJSPAWN = 1041,
	PKT_S_AI_KNOCKS_BACK = 1042,
	PKT_C_AI_KNOCKS_BACK = 1043,
	PKT_S_PLAYERSKILL_RANGE = 1044,
	PKT_C_PLAYERSKILL_RANGE = 1045,
	PKT_S_PLAYERSKILL_GUARD = 1046,
	PKT_C_PLAYERSKILL_GUARD = 1047,
	PKT_S_PLAYERSKILL_HEAL = 1048,
	PKT_C_PLAYERSKILL_HEAL = 1049,
	PKT_S_PLAYERHEAL = 1050,
	PKT_C_PLAYERHEAL = 1051,
	PKT_S_MAKEDRONE = 1052,
	PKT_C_MAKEDRONE = 1053,
	PKT_S_MOVEDRONE = 1054,
	PKT_C_MOVEDRONE = 1055,
	PKT_S_SEARCHDRONE = 1056,
	PKT_C_SEARCHDRONE = 1057,
	PKT_S_RETURNDRONE = 1058,
	PKT_C_RETURNDRONE = 1059,
};

// Custom Handlers
bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len);
bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt);
bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt);
bool Handle_S_LEAVE_GAME(PacketSessionRef& session, Protocol::S_LEAVE_GAME& pkt);
bool Handle_S_SPAWN(PacketSessionRef& session, Protocol::S_SPAWN& pkt);
bool Handle_S_DESPAWN(PacketSessionRef& session, Protocol::S_DESPAWN& pkt);
bool Handle_S_MOVE(PacketSessionRef& session, Protocol::S_MOVE& pkt);
bool Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt);
bool Handle_S_FIRE(PacketSessionRef& session, Protocol::S_FIRE& pkt);
bool Handle_S_HIT(PacketSessionRef& session, Protocol::S_HIT& pkt);
bool Handle_S_AISPAWN_RANDOM(PacketSessionRef& session, Protocol::S_AISPAWN_RANDOM& pkt);
bool Handle_S_AISPAWN_PATROL(PacketSessionRef& session, Protocol::S_AISPAWN_PATROL& pkt);
bool Handle_S_AISPAWN_BOSS(PacketSessionRef& session, Protocol::S_AISPAWN_BOSS& pkt);
bool Handle_S_AIMOVE(PacketSessionRef& session, Protocol::S_AIMOVE& pkt);
bool Handle_S_AIMOVESTOP(PacketSessionRef& session, Protocol::S_AIMOVESTOP& pkt);
bool Handle_S_AIATTACK(PacketSessionRef& session, Protocol::S_AIATTACK& pkt);
bool Handle_S_AIROTATE(PacketSessionRef& session, Protocol::S_AIROTATE& pkt);
bool Handle_S_AIHIT(PacketSessionRef& session, Protocol::S_AIHIT& pkt);
bool Handle_S_DAMAGED(PacketSessionRef& session, Protocol::S_DAMAGED& pkt);
bool Handle_S_AIDAMAGED(PacketSessionRef& session, Protocol::S_AIDAMAGED& pkt);
bool Handle_S_AIDEAD(PacketSessionRef& session, Protocol::S_AIDEAD& pkt);
bool Handle_S_PLAYERDEAD(PacketSessionRef& session, Protocol::S_PLAYERDEAD& pkt);
bool Handle_S_AIPROJSPAWN(PacketSessionRef& session, Protocol::S_AIPROJSPAWN& pkt);
bool Handle_S_AI_KNOCKS_BACK(PacketSessionRef& session, Protocol::S_AI_KNOCKS_BACK& pkt);
bool Handle_S_PLAYERSKILL_RANGE(PacketSessionRef& session, Protocol::S_PLAYERSKILL_RANGE& pkt);
bool Handle_S_PLAYERSKILL_GUARD(PacketSessionRef& session, Protocol::S_PLAYERSKILL_GUARD& pkt);
bool Handle_S_PLAYERSKILL_HEAL(PacketSessionRef& session, Protocol::S_PLAYERSKILL_HEAL& pkt);
bool Handle_S_PLAYERHEAL(PacketSessionRef& session, Protocol::S_PLAYERHEAL& pkt);
bool Handle_S_MAKEDRONE(PacketSessionRef& session, Protocol::S_MAKEDRONE& pkt);
bool Handle_S_MOVEDRONE(PacketSessionRef& session, Protocol::S_MOVEDRONE& pkt);
bool Handle_S_SEARCHDRONE(PacketSessionRef& session, Protocol::S_SEARCHDRONE& pkt);
bool Handle_S_RETURNDRONE(PacketSessionRef& session, Protocol::S_RETURNDRONE& pkt);

class ClientPacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID;
		GPacketHandler[PKT_S_LOGIN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_LOGIN>(Handle_S_LOGIN, session, buffer, len); };
		GPacketHandler[PKT_S_ENTER_GAME] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_ENTER_GAME>(Handle_S_ENTER_GAME, session, buffer, len); };
		GPacketHandler[PKT_S_LEAVE_GAME] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_LEAVE_GAME>(Handle_S_LEAVE_GAME, session, buffer, len); };
		GPacketHandler[PKT_S_SPAWN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SPAWN>(Handle_S_SPAWN, session, buffer, len); };
		GPacketHandler[PKT_S_DESPAWN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_DESPAWN>(Handle_S_DESPAWN, session, buffer, len); };
		GPacketHandler[PKT_S_MOVE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_MOVE>(Handle_S_MOVE, session, buffer, len); };
		GPacketHandler[PKT_S_CHAT] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_CHAT>(Handle_S_CHAT, session, buffer, len); };
		GPacketHandler[PKT_S_FIRE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_FIRE>(Handle_S_FIRE, session, buffer, len); };
		GPacketHandler[PKT_S_HIT] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_HIT>(Handle_S_HIT, session, buffer, len); };
		GPacketHandler[PKT_S_AISPAWN_RANDOM] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_AISPAWN_RANDOM>(Handle_S_AISPAWN_RANDOM, session, buffer, len); };
		GPacketHandler[PKT_S_AISPAWN_PATROL] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_AISPAWN_PATROL>(Handle_S_AISPAWN_PATROL, session, buffer, len); };
		GPacketHandler[PKT_S_AISPAWN_BOSS] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_AISPAWN_BOSS>(Handle_S_AISPAWN_BOSS, session, buffer, len); };
		GPacketHandler[PKT_S_AIMOVE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_AIMOVE>(Handle_S_AIMOVE, session, buffer, len); };
		GPacketHandler[PKT_S_AIMOVESTOP] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_AIMOVESTOP>(Handle_S_AIMOVESTOP, session, buffer, len); };
		GPacketHandler[PKT_S_AIATTACK] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_AIATTACK>(Handle_S_AIATTACK, session, buffer, len); };
		GPacketHandler[PKT_S_AIROTATE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_AIROTATE>(Handle_S_AIROTATE, session, buffer, len); };
		GPacketHandler[PKT_S_AIHIT] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_AIHIT>(Handle_S_AIHIT, session, buffer, len); };
		GPacketHandler[PKT_S_DAMAGED] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_DAMAGED>(Handle_S_DAMAGED, session, buffer, len); };
		GPacketHandler[PKT_S_AIDAMAGED] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_AIDAMAGED>(Handle_S_AIDAMAGED, session, buffer, len); };
		GPacketHandler[PKT_S_AIDEAD] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_AIDEAD>(Handle_S_AIDEAD, session, buffer, len); };
		GPacketHandler[PKT_S_PLAYERDEAD] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_PLAYERDEAD>(Handle_S_PLAYERDEAD, session, buffer, len); };
		GPacketHandler[PKT_S_AIPROJSPAWN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_AIPROJSPAWN>(Handle_S_AIPROJSPAWN, session, buffer, len); };
		GPacketHandler[PKT_S_AI_KNOCKS_BACK] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_AI_KNOCKS_BACK>(Handle_S_AI_KNOCKS_BACK, session, buffer, len); };
		GPacketHandler[PKT_S_PLAYERSKILL_RANGE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_PLAYERSKILL_RANGE>(Handle_S_PLAYERSKILL_RANGE, session, buffer, len); };
		GPacketHandler[PKT_S_PLAYERSKILL_GUARD] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_PLAYERSKILL_GUARD>(Handle_S_PLAYERSKILL_GUARD, session, buffer, len); };
		GPacketHandler[PKT_S_PLAYERSKILL_HEAL] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_PLAYERSKILL_HEAL>(Handle_S_PLAYERSKILL_HEAL, session, buffer, len); };
		GPacketHandler[PKT_S_PLAYERHEAL] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_PLAYERHEAL>(Handle_S_PLAYERHEAL, session, buffer, len); };
		GPacketHandler[PKT_S_MAKEDRONE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_MAKEDRONE>(Handle_S_MAKEDRONE, session, buffer, len); };
		GPacketHandler[PKT_S_MOVEDRONE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_MOVEDRONE>(Handle_S_MOVEDRONE, session, buffer, len); };
		GPacketHandler[PKT_S_SEARCHDRONE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SEARCHDRONE>(Handle_S_SEARCHDRONE, session, buffer, len); };
		GPacketHandler[PKT_S_RETURNDRONE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_RETURNDRONE>(Handle_S_RETURNDRONE, session, buffer, len); };
	}

	static bool HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}
	static SendBufferRef MakeSendBuffer(Protocol::C_LOGIN& pkt) { return MakeSendBuffer(pkt, PKT_C_LOGIN); }
	static SendBufferRef MakeSendBuffer(Protocol::C_ENTER_GAME& pkt) { return MakeSendBuffer(pkt, PKT_C_ENTER_GAME); }
	static SendBufferRef MakeSendBuffer(Protocol::C_LEAVE_GAME& pkt) { return MakeSendBuffer(pkt, PKT_C_LEAVE_GAME); }
	static SendBufferRef MakeSendBuffer(Protocol::C_MOVE& pkt) { return MakeSendBuffer(pkt, PKT_C_MOVE); }
	static SendBufferRef MakeSendBuffer(Protocol::C_CHAT& pkt) { return MakeSendBuffer(pkt, PKT_C_CHAT); }
	static SendBufferRef MakeSendBuffer(Protocol::C_FIRE& pkt) { return MakeSendBuffer(pkt, PKT_C_FIRE); }
	static SendBufferRef MakeSendBuffer(Protocol::C_HIT& pkt) { return MakeSendBuffer(pkt, PKT_C_HIT); }
	static SendBufferRef MakeSendBuffer(Protocol::C_AISPAWN_RANDOM& pkt) { return MakeSendBuffer(pkt, PKT_C_AISPAWN_RANDOM); }
	static SendBufferRef MakeSendBuffer(Protocol::C_AISPAWN_PATROL& pkt) { return MakeSendBuffer(pkt, PKT_C_AISPAWN_PATROL); }
	static SendBufferRef MakeSendBuffer(Protocol::C_AISPAWN_BOSS& pkt) { return MakeSendBuffer(pkt, PKT_C_AISPAWN_BOSS); }
	static SendBufferRef MakeSendBuffer(Protocol::C_AIMOVE& pkt) { return MakeSendBuffer(pkt, PKT_C_AIMOVE); }
	static SendBufferRef MakeSendBuffer(Protocol::C_AIMOVESTOP& pkt) { return MakeSendBuffer(pkt, PKT_C_AIMOVESTOP); }
	static SendBufferRef MakeSendBuffer(Protocol::C_AIATTACK& pkt) { return MakeSendBuffer(pkt, PKT_C_AIATTACK); }
	static SendBufferRef MakeSendBuffer(Protocol::C_AIROTATE& pkt) { return MakeSendBuffer(pkt, PKT_C_AIROTATE); }
	static SendBufferRef MakeSendBuffer(Protocol::C_AIHIT& pkt) { return MakeSendBuffer(pkt, PKT_C_AIHIT); }
	static SendBufferRef MakeSendBuffer(Protocol::C_DAMAGED& pkt) { return MakeSendBuffer(pkt, PKT_C_DAMAGED); }
	static SendBufferRef MakeSendBuffer(Protocol::C_AIDAMAGED& pkt) { return MakeSendBuffer(pkt, PKT_C_AIDAMAGED); }
	static SendBufferRef MakeSendBuffer(Protocol::C_AIDEAD& pkt) { return MakeSendBuffer(pkt, PKT_C_AIDEAD); }
	static SendBufferRef MakeSendBuffer(Protocol::C_PLAYERDEAD& pkt) { return MakeSendBuffer(pkt, PKT_C_PLAYERDEAD); }
	static SendBufferRef MakeSendBuffer(Protocol::C_AIPROJSPAWN& pkt) { return MakeSendBuffer(pkt, PKT_C_AIPROJSPAWN); }
	static SendBufferRef MakeSendBuffer(Protocol::C_AI_KNOCKS_BACK& pkt) { return MakeSendBuffer(pkt, PKT_C_AI_KNOCKS_BACK); }
	static SendBufferRef MakeSendBuffer(Protocol::C_PLAYERSKILL_RANGE& pkt) { return MakeSendBuffer(pkt, PKT_C_PLAYERSKILL_RANGE); }
	static SendBufferRef MakeSendBuffer(Protocol::C_PLAYERSKILL_GUARD& pkt) { return MakeSendBuffer(pkt, PKT_C_PLAYERSKILL_GUARD); }
	static SendBufferRef MakeSendBuffer(Protocol::C_PLAYERSKILL_HEAL& pkt) { return MakeSendBuffer(pkt, PKT_C_PLAYERSKILL_HEAL); }
	static SendBufferRef MakeSendBuffer(Protocol::C_PLAYERHEAL& pkt) { return MakeSendBuffer(pkt, PKT_C_PLAYERHEAL); }
	static SendBufferRef MakeSendBuffer(Protocol::C_MAKEDRONE& pkt) { return MakeSendBuffer(pkt, PKT_C_MAKEDRONE); }
	static SendBufferRef MakeSendBuffer(Protocol::C_MOVEDRONE& pkt) { return MakeSendBuffer(pkt, PKT_C_MOVEDRONE); }
	static SendBufferRef MakeSendBuffer(Protocol::C_SEARCHDRONE& pkt) { return MakeSendBuffer(pkt, PKT_C_SEARCHDRONE); }
	static SendBufferRef MakeSendBuffer(Protocol::C_RETURNDRONE& pkt) { return MakeSendBuffer(pkt, PKT_C_RETURNDRONE); }

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketType pkt;
		if (pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)) == false)
			return false;

		return func(session, pkt);
	}

	template<typename T>
	static SendBufferRef MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

#if UE_BUILD_DEBUG + UE_BUILD_DEVELOPMENT + UE_BUILD_TEST + UE_BUILD_SHIPPING >= 1
		SendBufferRef sendBuffer = MakeShared<SendBuffer>(packetSize);
#else
		SendBufferRef sendBuffer = GSendBufferManager->Open(packetSize);
#endif
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;
		
#if UE_BUILD_DEBUG + UE_BUILD_DEVELOPMENT + UE_BUILD_TEST + UE_BUILD_SHIPPING >= 1
		pkt.SerializeToArray(&header[1], dataSize);
#else
		ASSERT_CRASH(pkt.SerializeToArray(&header[1], dataSize));
#endif		
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};