#include "NetworkManager.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "SocketSubsystem.h"
#include "PacketSession.h"
#include "Protocol.pb.h"
#include "ClientPacketHandler.h"
#include "Kismet/GameplayStatics.h"
#include "Team_AIGameMode.h"
#include "GameCharacter.h"
//#include "Projectile.h"
//#include "CharacterController.h"
//#include "Weapon.h"
//#include "Team_AICharacter_Recv.h"

//#include "GenericPlatform/GenericPlatformMisc.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RunExternalExe.h"
#include "ChooseHostWidget.h"


UNetworkManager::UNetworkManager()
{
	//auto BP_MyCharacter = ConstructorHelpers::FClassFinder<AGameCharacter>(TEXT("/Game/BP/MyGameCharacter.MyGameCharacter_C"));
	//if (BP_MyCharacter.Succeeded())
	//	PlayerClass = BP_MyCharacter.Class;
	//auto BP_AIEnemy = ConstructorHelpers::FClassFinder<ATeam_AICharacterBase>(TEXT("/Game/Level_TeamMap/Blueprint/BP_AIEnemy_05.BP_AIEnemy_05_C"));
	//if (BP_AIEnemy.Succeeded())
	//	AIClass = BP_AIEnemy.Class;

	////TODO : use for 01 ~ 05, Boss
	//auto BP_AIEnemyRecv = ConstructorHelpers::FClassFinder<ATeam_AICharacter_Recv>(TEXT("/Game/Level_TeamMap/Blueprint/BP_AIEnemy_Recv.BP_AIEnemy_Recv_C"));
	//if (BP_AIEnemyRecv.Succeeded())
	//	AIClassRecv = BP_AIEnemyRecv.Class;
	externalExe = NewObject<URunExternalExe>();
}

FString UNetworkManager::GetLocalIPAddress()
{
	bool bCanBindAll;
	TSharedPtr<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBindAll);

	if (Addr.IsValid())
	{
		return Addr->ToString(false);
	}

	return FString("Unable to determine IP address");
}

bool UNetworkManager::ConnectToGameServer(FString ip)
{
	IpAddress = ip;
	//FString temp = GetLocalIPAddress();
	if (!SetGameMode(Cast<ATeam_AIGameMode>(GetWorld()->GetAuthGameMode())))
		return false;
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress, Ip);

	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connecting To Server...")));

	bool Connected = Socket->Connect(*InternetAddr);

	if (Connected)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Success")));

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
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Failed")));
	}
	return Connected;
}

bool UNetworkManager::SetGameMode(ATeam_AIGameMode* worldGameMode)
{
	GameMode = worldGameMode;

	return true;
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

void UNetworkManager::ExitGame()
{
	//FGenericPlatformMisc::RequestExit(false);

	GameServerSession->Disconnect();

	if (Socket)
	{
		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get();
		SocketSubsystem->DestroySocket(Socket);
		Socket = nullptr;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, false);
		}
	}
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

void UNetworkManager::HandlePlayerCount(const Protocol::S_PLAYERCOUNT& playerCount)
{
	if (!GameMode)
		return;

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameMode->SetPlayerCount(playerCount);
}

void UNetworkManager::HandleSpawn(const Protocol::ObjectInfo& ObjectInfo, bool IsMine)
{
	if (!GameMode)
		return;

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	if (!ObjectInfo.object_id())
		return;

	if (GameMode->GetPlayers().Find(ObjectInfo.object_id()))
		return;

	GameMode->SetPlayerInfo(IsMine, ObjectInfo.pos_info(), ObjectInfo.spawnidx());
}

void UNetworkManager::HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt)
{
	if (!GameMode)
		return;

	HandleSpawn(EnterGamePkt.player(), true);

	auto& myPlayer = GameMode->GetMyPlayer();
	if (!myPlayer)
		return;
	GameMode->InitializeStartPlay();
}

void UNetworkManager::HandleSpawn(const Protocol::S_SPAWN& SpawnPkt)
{
	for (auto& Player : SpawnPkt.players())
	{
		HandleSpawn(Player, false);
	}
}
/*
void UNetworkManager::HandleDespawn(uint64 ObjectId)
{
	//if (Socket == nullptr || GameServerSession == nullptr)
	//	return;

	//auto* World = GetWorld();
	//if (World == nullptr)
	//	return;

	//AGameCharacter** FindActor = Players.Find(ObjectId);
	//if (FindActor == nullptr)
	//	return;

	//World->DestroyActor(*FindActor);

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	if (!GameMode)
		return;

	auto FindActor = GameMode->GetPlayers()[ObjectId];
	if (FindActor == nullptr)
		return;

	World->DestroyActor(Cast<AActor>(FindActor));
}
*/
void UNetworkManager::HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;
	if (!GameMode)
		return;
	for (auto& ObjectId : DespawnPkt.object_ids())
	{
		GameMode->SetPlayerDespawn(ObjectId);
		//HandleDespawn(ObjectId);
	}
}

void UNetworkManager::HandleLeaveGame(const Protocol::S_LEAVE_GAME& leavePkt)
{
	GameServerSession->Disconnect();
	if (Socket)
	{
		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get();
		SocketSubsystem->DestroySocket(Socket);
		Socket = nullptr;
	}
}

void UNetworkManager::HandleMove(const Protocol::S_MOVE& MovePkt)
{
	if (!GameMode)
		return;

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameMode->SetPlayerMove(MovePkt);
}

void UNetworkManager::HandleAttack(const Protocol::S_FIRE& FirePkt)
{
	if (!GameMode)
		return;
	if (Socket == nullptr || GameServerSession == nullptr)
		return;
	GameMode->SetPlayerAttack(FirePkt);
}

void UNetworkManager::HandleHit(const Protocol::S_HIT& HitPkt)
{
	/*if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = HitPkt.targetid();
	AGameCharacter** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;*/

		// TODO : 맞은플레이어 데미지 입는 작업
		//AGameCharacter* TargetPlayer = *FindActor;
		//AActor* TargetActor = Cast<AActor>(*FindActor);
		//TSubclassOf<UDamageType> const DmgType = UDamageType::StaticClass();
		//FDamageEvent DmgEvent(DmgType);
		//TargetPlayer->TakeDamage(TargetPlayer->GetDamage(), DmgType ,TargetActor->GetInstigatorController(), )

	if (!GameMode)
		return;
	if (Socket == nullptr || GameServerSession == nullptr)
		return;
	/*GameMode->*/
}

void UNetworkManager::HandleDamaged(const Protocol::S_DAMAGED& dmgPkt)
{
	if (!GameMode)
		return;
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameMode->SetPlayerDamaged(dmgPkt);
}

void UNetworkManager::HandleDead(const Protocol::S_PLAYERDEAD& playerDeadPkt)
{
	if (!GameMode)
		return;
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameMode->SetPlayerDead(playerDeadPkt);
}

void UNetworkManager::HandleExpUP(const Protocol::S_EXPUP& expUpPkt)
{
	if (!GameMode)
		return;
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameMode->SetPlayerExpUP(expUpPkt);
}

void UNetworkManager::HandleLvUP(const Protocol::S_LVUP& lvUpPkt)
{
	if (!GameMode)
		return;
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameMode->SetPlayerLVUP(lvUpPkt);
}

void UNetworkManager::HandleSkillBomb(const Protocol::S_PLAYERSKILL_BOMB& bombPkt)
{
	if (!GameMode)
		return;
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameMode->SetPlayerSKill(bombPkt);
}

void UNetworkManager::HandleSkillChemical(const Protocol::S_PLAYERSKILL_CHEMICAL& chemPkt)
{
	if (!GameMode)
		return;
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameMode->SetPlayerSKill(chemPkt);
}

void UNetworkManager::HandleSkillGranade(const Protocol::S_PLAYERSKILL_GRANADE& granadePkt)
{
	if (!GameMode)
		return;
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameMode->SetPlayerSKill(granadePkt);
}

void UNetworkManager::HandleSkillGuard(const Protocol::S_PLAYERSKILL_GUARD& guardSkillPkt)
{
	if (!GameMode)
		return;
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameMode->SetPlayerSKill(guardSkillPkt);
}

void UNetworkManager::HandleSkillHeal(const Protocol::S_PLAYERSKILL_HEAL& healSkillPkt)
{
	if (!GameMode)
		return;
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameMode->SetPlayerSKill(healSkillPkt);
}

void UNetworkManager::HandleHealed(const Protocol::S_PLAYERHEAL& healpkt)
{
	if (!GameMode)
		return;
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameMode->SetUpdatedHeal(healpkt);
}

void UNetworkManager::HandleMakeDrone(const Protocol::S_MAKEDRONE& makeDrnPkt)
{
	if (!GameMode)
		return;
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameMode->SetMakeDrone(makeDrnPkt);
}

void UNetworkManager::HandleSearchDrone(const Protocol::S_SEARCHDRONE& searchDrnPkt)
{
	if (!GameMode)
		return;
	if (Socket == nullptr || GameServerSession == nullptr)
		return;
	GameMode->SetSearchDrone(searchDrnPkt);
}

void UNetworkManager::HandleMoveDrone(const Protocol::S_MOVEDRONE& moveDrnPkt)
{
	if (!GameMode)
		return;
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameMode->SetMoveDrone(moveDrnPkt);
}

void UNetworkManager::HandleReturnDrone(const Protocol::S_RETURNDRONE& retDrnPkt)
{
	if (!GameMode)
		return;
	if (Socket == nullptr || GameServerSession == nullptr)
		return;
	GameMode->SetReturnDrone(retDrnPkt);
}

void UNetworkManager::HandleAttackDrone(const Protocol::S_ATTACKDRONE& atkDrnPkt)
{
	if (!GameMode)
		return;
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameMode->SetAttackDrone(atkDrnPkt);
}

void UNetworkManager::HandleEatItemMaxHP(const Protocol::S_EATITEM_MAXHPUP& maxHpPkt)
{
	if (!GameMode)
		return;
	if (Socket == nullptr || GameServerSession == nullptr)
		return;
	GameMode->SetEatItemMaxHP(maxHpPkt);
}

void UNetworkManager::HandleEatItemLVUP(const Protocol::S_EATITEM_LVUP& lvPkt)
{
	if (!GameMode)
		return;
	if (Socket == nullptr || GameServerSession == nullptr)
		return;
	GameMode->SetEatItemLVUP(lvPkt);
}

void UNetworkManager::HandleEatItemDmgUP(const Protocol::S_EATITEM_DMGUP& dmgUpPkt)
{
	if (!GameMode)
		return;
	if (Socket == nullptr || GameServerSession == nullptr)
		return;
	GameMode->SetEatItemDmgUP(dmgUpPkt);
}

void UNetworkManager::HandleEatItemHealUp(const Protocol::S_EATITEM_HEALHP& healedPkt)
{
	if (!GameMode)
		return;
	if (Socket == nullptr || GameServerSession == nullptr)
		return;
	GameMode->SetEatItemHealUP(healedPkt);
}

void UNetworkManager::HandleAISpawn(const Protocol::S_AISPAWN_RANDOM& AiSpawnPkt)
{
	if (!GameMode)
		return;

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	if (GameMode->GetEnemies().Find(AiSpawnPkt.object_id()) != nullptr)
		return;

	GameMode->AddSpawnActor(AiSpawnPkt);
}

void UNetworkManager::HandleAISpawn(const Protocol::S_AISPAWN_PATROL& AiSpawnPkt)
{
	if (!GameMode)
		return;

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	if (GameMode->GetEnemies().Find(AiSpawnPkt.object_id()) != nullptr)
		return;

	GameMode->AddSpawnActor(AiSpawnPkt);
}

void UNetworkManager::HandleAISpawn(const Protocol::S_AISPAWN_BOSS& AiSpawnPkt)
{
	if (!GameMode)
		return;

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	if (GameMode->GetEnemies().Find(AiSpawnPkt.object_id()) != nullptr)
		return;

	GameMode->AddSpawnActor(AiSpawnPkt);
}

/*
void UNetworkManager::HandleAISpawn(const Protocol::ObjectInfo& AiInfo)
{
	if (!GameMode)
		return;

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	if (GameMode->GetEnemies().Find(AiInfo.object_id()))
		return;

	GameMode->AddSpawnActor(AiInfo);
}
*/

void UNetworkManager::HandleAIMove(const Protocol::S_AIMOVE& AIMovePkt)
{
	if (!GameMode)
		return;

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameMode->SetAIMove(AIMovePkt);

	//auto* World = GetWorld();
	//if (World == nullptr)
	//	return;

	//const uint64 ObjectId = AIMovePkt.info().object_id();
	//ATeam_AICharacterBase** FindActor = Enemies.Find(ObjectId);
	//if (FindActor == nullptr)
	//	return;

	////ATeam_AICharacterBase* enemy = (*FindActor);
	//ATeam_AICharacter_Recv* enemy = Cast<ATeam_AICharacter_Recv>(*FindActor);
	//if (enemy == nullptr)
	//	return;
	//const Protocol::PosInfo& Info = AIMovePkt.info();
	//enemy->pos = Info;
	//FVector loc{ enemy->pos.x(), enemy->pos.y(), enemy->pos.z() };
	//FRotator rot{ enemy->pos.pitch(), enemy->pos.yaw(), enemy->pos.roll() };
	//*enemy->SetCurrentSpeed(enemy->pos.speed());
	//enemy->SetActorLocation(loc);
	//enemy->SetActorRotation(rot);*/
	//enemy->RecvMoveTo(loc, rot, enemy->pos.speed());
}

void UNetworkManager::HandleAIMoveStop(const Protocol::S_AIMOVESTOP& AIMoveStopPkt)
{
	if (!GameMode)
		return;

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameMode->SetAIMoveStop(AIMoveStopPkt);

	//auto* World = GetWorld();
	//if (World == nullptr)
	//	return;

	//const uint64 ObjectId = AIMoveStopPkt.object_id();
	//ATeam_AICharacterBase** FindActor = Enemies.Find(ObjectId);
	//if (FindActor == nullptr)
	//	return;

	//ATeam_AICharacter_Recv* enemy = Cast<ATeam_AICharacter_Recv>(*FindActor);
	//if (enemy == nullptr)
	//	return;
	///*enemy->pos.set_speed(AIMoveStopPkt.speed());
	//enemy->SetCurrentSpeed(enemy->pos.speed());*/
	//enemy->RecvMoveTo(enemy->GetActorLocation(), enemy->GetActorRotation(), enemy->pos.speed());
}

void UNetworkManager::HandleAIAttack(const Protocol::S_AIATTACK& AIattackPkt)
{
	if (!GameMode)
		return;

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	GameMode->SetAIAttack(AIattackPkt);

	//const uint64 ObjectId = AIattackPkt.object_id();
	//ATeam_AICharacterBase** FindActor = Enemies.Find(ObjectId);
	//if (FindActor == nullptr)
	//	return;
	//ATeam_AICharacter_Recv* enemy = Cast<ATeam_AICharacter_Recv>(*FindActor);
	//if (enemy == nullptr)
	//	return;
	//enemy->RecvAttack();
}

void UNetworkManager::HandleAIAttackBoss(const Protocol::S_AIATTACK_BOSS2& aiAttackBossPkt)
{
	if (!GameMode)
		return;

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	GameMode->SetAIBossAttack2(aiAttackBossPkt);
}

void UNetworkManager::HandleAIRotate(const Protocol::S_AIROTATE& AIRotPkt)
{
	if (!GameMode)
		return;

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	GameMode->SetAIRotate(AIRotPkt);
}

void UNetworkManager::HandleAIHit(const Protocol::S_AIHIT& AIHitPkt)
{
	if (!GameMode)
		return;

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	GameMode->SetAIHit(AIHitPkt);
}

void UNetworkManager::HandleAISpawnProjectile(const Protocol::S_AIPROJSPAWN& AIProjPkt)
{
	if (!GameMode)
		return;

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	GameMode->SetAISpawnProj(AIProjPkt);
}

void UNetworkManager::HandleAIDamaged(const Protocol::S_AIDAMAGED& AIDmgedPkt)
{
	if (!GameMode)
		return;

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	GameMode->SetAIDamaged(AIDmgedPkt);
}

void UNetworkManager::HandleAIDead(const Protocol::S_AIDEAD& AIDeadPkt)
{
	if (!GameMode)
		return;

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	GameMode->SetAIDead(AIDeadPkt);
}

void UNetworkManager::HandleAIKnocksBack(const Protocol::S_AI_KNOCKS_BACK& AIKnocksBock)
{
	if (!GameMode)
		return;

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	GameMode->SetKnocksBack(AIKnocksBock);
}

void UNetworkManager::HandleRmvStartWidget(const Protocol::S_GAMESTART& pkt)
{
	if (!GameMode)
		return;

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	GameMode->RemoveStartWidget();
}

void UNetworkManager::HandleSetMagneticField(const Protocol::S_SET_MAGNETICFIELD& magPkt)
{
	if (!GameMode)
		return;

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	GameMode->SetMagneticField(magPkt);
}

void UNetworkManager::HandleSetWorldLV(const Protocol::S_WORLD_LVUP& worldlvPkt)
{
	if (!GameMode)
		return;

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	GameMode->SetGameLevel(worldlvPkt.worldlevel());
}

void UNetworkManager::ChangeMainBGM(const Protocol::S_GAMESTART& pkt)
{
	if (!GameMode)
		return;

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	GameMode->StopAudioSystem(TEXT("BGM_Wait"));
	GameMode->PlayAudioSystem(TEXT("BGM_Main"));
}

void UNetworkManager::HandleGameResult(const Protocol::S_GAMERESULT& pkt)
{
	if (!GameMode)
		return;

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	GameMode->ViewGameResult(pkt);
}
