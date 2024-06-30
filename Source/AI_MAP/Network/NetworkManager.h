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

	FString GetLocalIPAddress();

	UFUNCTION(BlueprintCallable)
	bool ConnectToGameServer(FString ip);

	UFUNCTION(BlueprintCallable)
	bool SetGameMode(ATeam_AIGameMode* worldGameMode);

	UFUNCTION(BlueprintCallable)
	void DisconnectFromGameServer();
	void ExitGame();

	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

	void SendPacket(SendBufferRef SendBuffer);

	template<typename T>
	void SendPacket(T packet) const;

public:
	//Player
	void HandlePlayerCount(const Protocol::S_PLAYERCOUNT& playerCount);
	void HandleSpawn(const Protocol::ObjectInfo& PlayerInfo, bool IsMine);
	void HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt);
	void HandleSpawn(const Protocol::S_SPAWN& SpawnPkt);

	//void HandleDespawn(uint64 ObjectId);
	void HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt);
	void HandleLeaveGame(const Protocol::S_LEAVE_GAME& leavePkt);

	void HandleMove(const Protocol::S_MOVE& MovePkt);

	void HandleAttack(const Protocol::S_FIRE& FirePkt);
	void HandleHit(const Protocol::S_HIT& HitPkt);
	void HandleDamaged(const Protocol::S_DAMAGED& dmgPkt);
	void HandleDead(const Protocol::S_PLAYERDEAD& playerDeadPkt);
	void HandleExpUP(const Protocol::S_EXPUP& expUpPkt);
	void HandleLvUP(const Protocol::S_LVUP& lvUpPkt);

	void HandleSkillBomb(const Protocol::S_PLAYERSKILL_BOMB& bombPkt);
	void HandleSkillChemical(const Protocol::S_PLAYERSKILL_CHEMICAL& chemPkt);
	void HandleSkillGranade(const Protocol::S_PLAYERSKILL_GRANADE& granadePkt);
	void HandleSkillGuard(const Protocol::S_PLAYERSKILL_GUARD& rangeSkillPkt);
	void HandleSkillHeal(const Protocol::S_PLAYERSKILL_HEAL& rangeSkillPkt);
	void HandleHealed(const Protocol::S_PLAYERHEAL& healpkt);

	void HandleMakeDrone(const Protocol::S_MAKEDRONE& makeDrnPkt);
	void HandleSearchDrone(const Protocol::S_SEARCHDRONE& searchDrnPkt);
	void HandleMoveDrone(const Protocol::S_MOVEDRONE& moveDrnPkt);
	void HandleReturnDrone(const Protocol::S_RETURNDRONE& retDrnPkt);
	void HandleAttackDrone(const Protocol::S_ATTACKDRONE& atkDrnPkt);

	void HandleEatItemMaxHP(const Protocol::S_EATITEM_MAXHPUP& maxHpPkt);
	void HandleEatItemLVUP(const Protocol::S_EATITEM_LVUP& lvPkt);
	void HandleEatItemDmgUP(const Protocol::S_EATITEM_DMGUP& dmgUpPkt);
	void HandleEatItemHealUp(const Protocol::S_EATITEM_HEALHP& healedPkt);

	// AI
	/*void HandleAISpawn(const Protocol::ObjectInfo& AiInfo);*/
	void HandleAISpawn(const Protocol::S_AISPAWN_RANDOM& AiSpawnPkt);
	void HandleAISpawn(const Protocol::S_AISPAWN_PATROL& AiSpawnPkt);
	void HandleAISpawn(const Protocol::S_AISPAWN_BOSS& AiSpawnPkt);

	void HandleAIMove(const Protocol::S_AIMOVE& AIMovePkt);
	void HandleAIMoveStop(const Protocol::S_AIMOVESTOP& AIMoveStopPkt);
	void HandleAIAttack(const Protocol::S_AIATTACK& AIattackPkt);
	void HandleAIAttackBoss(const Protocol::S_AIATTACK_BOSS2& aiAttackBossPkt);
	void HandleAIRotate(const Protocol::S_AIROTATE& AIRotPkt);
	void HandleAIHit(const Protocol::S_AIHIT& AIHitPkt);
	void HandleAISpawnProjectile(const Protocol::S_AIPROJSPAWN& AIProjPkt);
	void HandleAIDamaged(const Protocol::S_AIDAMAGED& AIDmgedPkt);
	void HandleAIDead(const Protocol::S_AIDEAD& AIDeadPkt);
	void HandleAIKnocksBack(const Protocol::S_AI_KNOCKS_BACK& AIKnocksBock);

	// Map
	void HandleRmvStartWidget(const Protocol::S_GAMESTART& pkt);
	void HandleSetMagneticField(const Protocol::S_SET_MAGNETICFIELD& magPkt);
	void HandleSetWorldLV(const Protocol::S_WORLD_LVUP& worldlvPkt);
	void ChangeMainBGM(const Protocol::S_GAMESTART& pkt);
	void HandleGameResult(const Protocol::S_GAMERESULT& pkt);
public:
	// GameServer
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1");
	int16 Port = 7777;

	TSharedPtr<class PacketSession> GameServerSession;
	uint64 ClientID;
	uint64 PlayerCount;
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class URunExternalExe> externalExe;
};


template <typename T> 
void UNetworkManager::SendPacket(T packet) const
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	const SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
	GameServerSession->SendPacket(SendBuffer);
}
