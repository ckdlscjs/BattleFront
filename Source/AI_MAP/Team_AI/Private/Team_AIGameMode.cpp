// Copyright Epic Games, Inc. All Rights Reserved.

#include "Team_AIGameMode.h"
#include "Team_AIPlayerController.h"
#include "Team_AICharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Team_AISpawnPointRandom.h"
#include "Team_AISpawnPointPatrol.h"
#include "Team_AIPatrolRoute.h"
#include "Team_AICharacterBase.h"
#include "Team_AIGameInstance.h"
#include "AbilityManager.h"
#include "GameCharacter.h"
#include "Projectile.h"
#include "CharacterController.h"
#include "Weapon.h"
#include "Team_AICharacter_Recv.h"
#include "Team_AIProjectileBase.h"
#include "Team_AISpawnPointBoss.h"
ATeam_AIGameMode::ATeam_AIGameMode()
{
	// use our custom PlayerController class

	// set default pawn class to our Blueprinted character

	/*
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("Blueprint'/Game/BP/BP_GameCharacter.BP_GameCharacter_C'"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("Blueprint'/Game/BP/BP_MyController.BP_MyController_C'"));
	if (PlayerControllerBPClass.Class != nullptr)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
	static ConstructorHelpers::FObjectFinder<UBlueprint> blueprint_finder(TEXT("/Game/Level_TeamMap/Blueprint/BP_AICharacterBase.BP_AICharacterBase"));
	if (blueprint_finder.Succeeded())
	{
		mBlueprintClass = (UClass*)blueprint_finder.Object->GeneratedClass;
	}
	*/
	Duration_SpawnEnemyRandom = 5.0f;
	Duration_SpawnEnemyPatrol = 5.0f;
	Duration_SpawnEnemyBoss = 5.0f;
	Duration_GameLevel = 15.0f;
	GameLevel = 1;

	//Server
	AIID = 100;
}


void ATeam_AIGameMode::StartPlay()
{
	Super::StartPlay();
	bAllowTickBeforeBeginPlay = true;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATeam_AISpawnPointRandom::StaticClass(), SpawnPoints_Random);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATeam_AIPatrolRoute::StaticClass(), PatrolRoutes);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATeam_AISpawnPointBoss::StaticClass(), SpawnPoints_Boss);

	////IncreaseGameLevel
	//GetWorldTimerManager().SetTimer
	//(
	//	TimerHandle_GameLevel,
	//	[this]() -> void
	//	{
	//		UTeam_AIGameInstance* GameInstance = Cast<UTeam_AIGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//		GameLevel = std::min(GameLevel + 1, GameInstance->GetSizeCharacterDataTable());
	//		if (GameLevel >= GameInstance->GetSizeCharacterDataTable())
	//			GetWorldTimerManager().ClearTimer(TimerHandle_GameLevel);
	//	},
	//	Duration_GameLevel,
	//	true
	//
	//);
	////SpawnEnemy
	//GetWorldTimerManager().SetTimer(TimerHandle_SpawnRandom, this, &ATeam_AIGameMode::SpawnEnemyRandom, Duration_SpawnEnemyRandom, true);
	//GetWorldTimerManager().SetTimer(TimerHandle_SpawnPatrol, this, &ATeam_AIGameMode::SpawnEnemyPatrol, Duration_SpawnEnemyPatrol, true);
}

void ATeam_AIGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


}

void ATeam_AIGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	GetNetworkManager()->DisconnectFromGameServer();
}



void ATeam_AIGameMode::SpawnEnemyRandom()
{
	if (SpawnPoints_Random.Num() <= 0)
		return;
	//TODO : Random Enemy
	//for (const auto& spawnPoint : SpawnPoints_Random)
	for (int i = 0; i<SpawnPoints_Random.Num();i++)
	{
		//auto enermy = Cast<ATeam_AISpawnPointRandom>(spawnPoint)->SpawnActor();
		auto enermy = Cast<ATeam_AISpawnPointRandom>(SpawnPoints_Random[i])->SpawnActor();
		Enemies.Add(enermy->pos.object_id(), enermy);
		//AddSpawnActor(enermy);
		//auto enermy = Cast<ATeam_AISpawnPointRandom>(spawnPoint);
		
		Protocol::C_AISPAWN_RANDOM pkt;
		{
			pkt.set_object_id(enermy->pos.object_id());
			pkt.set_container_idx(i);
			GetNetworkManager()->SendPacket(pkt); 
		}
	}
}

void ATeam_AIGameMode::SpawnEnemyPatrol()
{
	if (PatrolRoutes.Num() <= 0)
		return;
	//TODO : Patrol Enemy
	//for (const auto& iter : PatrolRoutes)
	for(int i = 0; i<PatrolRoutes.Num(); i++)
	{
		//ATeam_AIPatrolRoute* patrolRoute = Cast<ATeam_AIPatrolRoute>(iter);
		ATeam_AIPatrolRoute* patrolRoute = Cast<ATeam_AIPatrolRoute>(PatrolRoutes[i]);
		if (patrolRoute->CheckSpawnPossible())
		{
			for (const auto& spawnPoint : patrolRoute->GetSpawnPoints())
			{
				/*AddSpawnActor(spawnPoint->SpawnActor());*/
				auto enermy = spawnPoint->SpawnActor();
				Enemies.Add(enermy->pos.object_id(), enermy);

				Protocol::C_AISPAWN_PATROL pkt;
				{
					pkt.set_object_id(enermy->pos.object_id());
					pkt.set_container_idx(i);
					GetNetworkManager()->SendPacket(pkt);
				}
			}
		}
	}
}

void ATeam_AIGameMode::SpawnEnemyBoss()
{
	if (SpawnPoints_Boss.Num() <= 0)
		return;
	if (Boss)
		return;

	for (int i = 0; i < SpawnPoints_Boss.Num(); i++)
	{		
		Boss = Cast<ATeam_AISpawnPointBoss>(SpawnPoints_Boss[i])->SpawnActor();
		Enemies.Add(Boss->pos.object_id(), Boss);
		Protocol::C_AISPAWN_BOSS pkt;
		{
			pkt.set_object_id(Boss->pos.object_id());
			pkt.set_container_idx(i);
			GetNetworkManager()->SendPacket(pkt);
		}
	}
	
}

int32 ATeam_AIGameMode::GetGameLevel() const
{
	return GameLevel;
}

void ATeam_AIGameMode::AddSpawnActor(const Protocol::S_AISPAWN_RANDOM& AiSpawnPkt)
{
	ATeam_AICharacter_Recv* enemy = nullptr;

	enemy = Cast<ATeam_AISpawnPointRandom>(SpawnPoints_Random[AiSpawnPkt.container_idx()])->RecvedSpawnActor();
	enemy->pos.set_object_id(AiSpawnPkt.object_id());
	Enemies.Add(AiSpawnPkt.object_id(), enemy);
}

void ATeam_AIGameMode::AddSpawnActor(const Protocol::S_AISPAWN_PATROL& AiSpawnPkt)
{
	ATeam_AICharacter_Recv* enemy = nullptr;

	enemy = Cast<ATeam_AISpawnPointPatrol>(PatrolRoutes[AiSpawnPkt.container_idx()])->RecvedSpawnActor();
	enemy->pos.set_object_id(AiSpawnPkt.object_id());
	Enemies.Add(AiSpawnPkt.object_id(), enemy);
}

void ATeam_AIGameMode::AddSpawnActor(const Protocol::S_AISPAWN_BOSS& AiSpawnPkt)
{
	Boss = Cast<ATeam_AISpawnPointBoss>(SpawnPoints_Boss[AiSpawnPkt.container_idx()])->RecvedSpawnActor();
	Boss->pos.set_object_id(AiSpawnPkt.object_id());
	Enemies.Add(AiSpawnPkt.object_id(), Boss);
}


float ATeam_AIGameMode::GetDurationSpawnEnemyRandom() const
{
	return Duration_SpawnEnemyRandom;
}

float ATeam_AIGameMode::GetDurationSpawnEnemyPatrol() const
{
	return Duration_SpawnEnemyPatrol;
}




TMap<uint64, AGameCharacter*>& ATeam_AIGameMode::GetPlayers()
{
	return Players;
}

void ATeam_AIGameMode::SetPlayerInfo(bool isMine, const Protocol::PosInfo& Info)
{
	FVector location(Info.x(), Info.y(), Info.z());
	FRotator rotation(Info.pitch(), Info.yaw(), Info.roll());
	AGameCharacter* player = nullptr;
	if (isMine)
	{
		// Case 1
		auto PlayerController = Cast<ACharacterController>(UGameplayStatics::GetPlayerController(this, 0));
		player = Cast<AGameCharacter>(GetWorld()->SpawnActor(PlayerClass, &location, &rotation));
		PlayerController->Possess(player);
		PlayerController->SetCharacter(Cast<AGameCharacter>(PlayerController->GetPawn()));
		

		// Case 2
		//auto PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		//player = Cast<AGameCharacter>(PlayerController->GetPawn());

		MyPlayer = player;
	}
	else
	{
		if(MyPlayer->PlayerInfo->object_id() != Info.object_id())
			player = Cast<AGameCharacter>(GetWorld()->SpawnActor(PlayerClass, &location, &rotation));
	}
	if (!player)
		return;
	player->SetPlayerInfo(Info);
	Players.Add(Info.object_id(), player);
}

const TObjectPtr<AGameCharacter>& ATeam_AIGameMode::GetMyPlayer() const
{
	// TODO: ���⿡ return ���� �����մϴ�.
	return MyPlayer;
}

void ATeam_AIGameMode::SetPlayerMove(const Protocol::S_MOVE& MovePkt)
{
	const uint64 ObjectId = MovePkt.info().object_id();
	AGameCharacter** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	AGameCharacter* Player = (*FindActor);

	const Protocol::PosInfo& Info = MovePkt.info();
	Player->SetDestInfo(Info);
}

void ATeam_AIGameMode::SetPlayerAttack(const Protocol::S_FIRE& FirePkt)
{
	const uint64 ObjectId = FirePkt.projectile().object_id();

	AGameCharacter** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	FVector Loc(FirePkt.projectile().x(), FirePkt.projectile().y(), FirePkt.projectile().z());
	FRotator Rot(FirePkt.projectile().pitch(), FirePkt.projectile().yaw(), FirePkt.projectile().roll());

	AGameCharacter* Player = *FindActor;
	Player->SetFightState(FirePkt.projectile().bfight());
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(Player->GetWeapon()->GetPrjClass(), Loc, Rot);
	Projectile->SetOwner(Player);
	Projectile->SetCollisionEnable(true);
	//add to container Projectiles_Players
}

void ATeam_AIGameMode::SetPlayerDamaged(const Protocol::S_DAMAGED& dmgPkt)
{
	const uint64 ObjectId = dmgPkt.object_id();

	AGameCharacter** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;
	AGameCharacter* Player = *FindActor;
	Player->UpdateHP(dmgPkt.hp());
}

void ATeam_AIGameMode::SetPlayerDead(const Protocol::S_PLAYERDEAD& playerDeadPkt)
{
	const uint64 ObjectId = playerDeadPkt.object_id();
	AGameCharacter** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;
	AGameCharacter* Player = *FindActor;
	Player->SetDead(playerDeadPkt.dead());
}

void ATeam_AIGameMode::SetPlayerSKill(const Protocol::S_PLAYERSKILL_RANGE& rangePkt)
{
	const uint64 ObjectId = rangePkt.object_id();
	AGameCharacter** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;
	AGameCharacter* Player = *FindActor;

	if (MyPlayer != Player)
	{
		auto am = Player->GetAbilityManger();
		FVector loc = { rangePkt.x(), rangePkt.y(), rangePkt.z() };
		am->RecvMakeRange(Player, rangePkt.abilityarrayidx(), loc);
	}
}

void ATeam_AIGameMode::SetPlayerSKill(const Protocol::S_PLAYERSKILL_GUARD& guardPkt)
{
	const uint64 ObjectId = guardPkt.object_id();
	AGameCharacter** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;
	AGameCharacter* Player = *FindActor;
	Player->SetGuardPoint(guardPkt.guardpoint());
}

void ATeam_AIGameMode::SetPlayerSKill(const Protocol::S_PLAYERSKILL_HEAL& healPkt)
{
	const uint64 ObjectId = healPkt.object_id();
	AGameCharacter** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;
	AGameCharacter* Player = *FindActor;
	auto am = Player->GetAbilityManger();
	am->RecvMakeHeal(healPkt.abilityarrayidx());
}

void ATeam_AIGameMode::SetUpdatedHeal(const Protocol::S_PLAYERHEAL& healPkt)
{
	const uint64 ObjectId = healPkt.object_id();
	AGameCharacter** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;
	AGameCharacter* Player = *FindActor;
	Player->UpdateHP(healPkt.updeatedhp());
}

void ATeam_AIGameMode::SetMakeDrone(const Protocol::S_MAKEDRONE& makeDrnPkt)
{
	const uint64 ObjectId = makeDrnPkt.object_id();
	AGameCharacter** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;
	AGameCharacter* Player = *FindActor;
	auto abilityManager = Player->GetAbilityManager();
	if (abilityManager)
	{
		auto drone = abilityManager->GetDrone();
		if (drone == nullptr)
			abilityManager->RecvMakeDrone(makeDrnPkt.abilityarrayidx(), Player);
	}
}

void ATeam_AIGameMode::SetSearchDrone(const Protocol::S_SEARCHDRONE& searchDrnPkt)
{
	const uint64 ObjectId = searchDrnPkt.object_id();
	AGameCharacter** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;
	AGameCharacter* Player = *FindActor;
	auto abilityManager = Player->GetAbilityManager();
	if (abilityManager == nullptr)
		return;
	FVector loc{ searchDrnPkt.x(), searchDrnPkt.y(), searchDrnPkt.z() };
	abilityManager->RecvSearchDrone(loc);
}

void ATeam_AIGameMode::SetMoveDrone(const Protocol::S_MOVEDRONE& moveDrnPkt)
{
	const uint64 ObjectId = moveDrnPkt.object_id();
	AGameCharacter** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;
	AGameCharacter* Player = *FindActor;
	auto abilityManager = Player->GetAbilityManager();
	if (abilityManager == nullptr)
		return;
	FVector loc{ moveDrnPkt.x(), moveDrnPkt.y(), moveDrnPkt.z() };
	abilityManager->RecvMoveDrone(loc);
}

const TMap<uint64, ATeam_AICharacterBase*>& ATeam_AIGameMode::GetEnemies() const
{
	return Enemies;
}

void ATeam_AIGameMode::SetAIMove(const Protocol::S_AIMOVE& AIMovePkt)
{
	const uint64 ObjectId = AIMovePkt.info().object_id();
	ATeam_AICharacterBase** FindActor = Enemies.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	//ATeam_AICharacterBase* enemy = (*FindActor);
	ATeam_AICharacter_Recv* enemy = Cast<ATeam_AICharacter_Recv>(*FindActor);
	if (enemy == nullptr)
		return;
	/*const Protocol::PosInfo& Info = AIMovePkt.info();*/
	enemy->pos = AIMovePkt.info();
	FVector loc{ enemy->pos.x(), enemy->pos.y(), enemy->pos.z() };
	FRotator rot{ enemy->pos.pitch(), enemy->pos.yaw(), enemy->pos.roll() };
	/*enemy->SetCurrentSpeed(enemy->pos.speed());
	enemy->SetActorLocation(loc);
	enemy->SetActorRotation(rot);*/
	enemy->RecvMoveTo(loc, rot, enemy->pos.speed());
}

void ATeam_AIGameMode::SetAIMoveStop(const Protocol::S_AIMOVESTOP& AIMoveStopPkt)
{
	const uint64 ObjectId = AIMoveStopPkt.object_id();
	ATeam_AICharacterBase** FindActor = Enemies.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	ATeam_AICharacter_Recv* enemy = Cast<ATeam_AICharacter_Recv>(*FindActor);
	if (enemy == nullptr)
		return;
	enemy->pos.set_speed(AIMoveStopPkt.speed());
	enemy->SetCurrentSpeed(enemy->pos.speed());
	//enemy->RecvMoveTo(enemy->GetActorLocation(), enemy->GetActorRotation(), enemy->pos.speed());
}

void ATeam_AIGameMode::SetAIAttack(const Protocol::S_AIATTACK& AIattackPkt)
{
	const uint64 ObjectId = AIattackPkt.object_id();
	ATeam_AICharacterBase** FindActor = Enemies.Find(ObjectId);
	if (FindActor == nullptr)
		return;
	ATeam_AICharacter_Recv* enemy = Cast<ATeam_AICharacter_Recv>(*FindActor);
	if (enemy == nullptr)
		return;
	enemy->RecvAttack(AIattackPkt.attack_idx());
}

void ATeam_AIGameMode::SetAIRotate(const Protocol::S_AIROTATE& AIRotPkt)
{
	const uint64 ObjectId = AIRotPkt.object_id();
	ATeam_AICharacterBase** FindActor = Enemies.Find(ObjectId);
	if (FindActor == nullptr)
		return;
	ATeam_AICharacter_Recv* enemy = Cast<ATeam_AICharacter_Recv>(*FindActor);
	if (enemy == nullptr)
		return;

	FRotator rot{ AIRotPkt.pitch(),AIRotPkt.yaw(),AIRotPkt.roll() };
	enemy->RecvRotate(rot);
	FVector loc{ AIRotPkt.x(), AIRotPkt.y(), AIRotPkt.z() };
	enemy->SetActorLocation(loc);
}

void ATeam_AIGameMode::SetAIHit(const Protocol::S_AIHIT& AIHitPkt)
{
	const uint64 ObjectId = AIHitPkt.object_id();
	ATeam_AICharacterBase** FindActor = Enemies.Find(ObjectId);
	if (FindActor == nullptr)
		return;
	ATeam_AICharacter_Recv* enemy = Cast<ATeam_AICharacter_Recv>(*FindActor);
	if (enemy == nullptr)
		return;

	//enemy->ActiveAttackParticle();
}

void ATeam_AIGameMode::SetAISpawnProj(const Protocol::S_AIPROJSPAWN& AIProjPkt)
{
	const uint64 ObjectId = AIProjPkt.object_id();
	ATeam_AICharacterBase** FindActor = Enemies.Find(ObjectId);
	if (FindActor == nullptr)
		return;
	ATeam_AICharacter_Recv* enemy = Cast<ATeam_AICharacter_Recv>(*FindActor);
	if (enemy == nullptr)
		return;
	//enemy->ActiveAttackParticle();
	FVector loc{ AIProjPkt.x(), AIProjPkt.y(), AIProjPkt.z() };
	FRotator rot{ AIProjPkt.pitch(), AIProjPkt.yaw(), AIProjPkt.roll() };
	ATeam_AIProjectileBase* Projectile = Cast<ATeam_AIProjectileBase>(GetWorld()->SpawnActor<AActor>(GetAIProjClasses()[AIProjPkt.projidx()], loc, rot));
	if (!Projectile)
		return;
	Projectile->SetOwner(enemy);
	Projectile->ProjectileInitialize(AIProjPkt.attackdmg(), AIProjPkt.speed(), AIProjPkt.speed(), 0.0f);
	Projectile->SetCollisionEnable(true);
}

void ATeam_AIGameMode::SetAIDamaged(const Protocol::S_AIDAMAGED& AIDmgedPkt)
{
	const uint64 ObjectId = AIDmgedPkt.object_id();
	ATeam_AICharacterBase** FindActor = Enemies.Find(ObjectId);
	if (FindActor == nullptr)
		return;
	ATeam_AICharacter_Recv* enemy = Cast<ATeam_AICharacter_Recv>(*FindActor);
	if (enemy == nullptr)
		return;

	enemy->SetCurHP(AIDmgedPkt.hp());
}

void ATeam_AIGameMode::SetAIDead(const Protocol::S_AIDEAD& AIDeadPkt)
{
	const uint64 ObjectId = AIDeadPkt.object_id();
	ATeam_AICharacterBase** FindActor = Enemies.Find(ObjectId);
	if (FindActor == nullptr)
		return;
	ATeam_AICharacter_Recv* enemy = Cast<ATeam_AICharacter_Recv>(*FindActor);
	if (enemy == nullptr)
		return;

	enemy->RecvDead();
}

void ATeam_AIGameMode::SetAIDespawn(uint64 objectID)
{
	Enemies.Remove(objectID);
}

void ATeam_AIGameMode::SetKnocksBack(const Protocol::S_AI_KNOCKS_BACK& AIKnocksBackPkt)
{
	const uint64 ObjectId = AIKnocksBackPkt.target_id();
	AGameCharacter** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	AGameCharacter* Player = (*FindActor);

	FVector VelVec{ AIKnocksBackPkt.vx(),AIKnocksBackPkt.vy(), AIKnocksBackPkt.vz() };
	Player->LaunchCharacter(VelVec, false, false);
	/*Player->DesLoc = Player->GetActorLocation();*/
}

UNetworkManager* ATeam_AIGameMode::GetNetworkManager() const
{
	return GetGameInstance()->GetSubsystem<UNetworkManager>();
}

void ATeam_AIGameMode::InitializeStartPlay()
{
	if (MyPlayer->PlayerInfo->object_id() != 1)
		return;
	//IncreaseGameLevel
	GetWorldTimerManager().SetTimer
	(
		TimerHandle_GameLevel,
		[this]() -> void
		{
			UTeam_AIGameInstance* GameInstance = Cast<UTeam_AIGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
			GameLevel = std::min(GameLevel + 1, GameInstance->GetSizeCharacterDataTable());
			if (GameLevel >= GameInstance->GetSizeCharacterDataTable())
				GetWorldTimerManager().ClearTimer(TimerHandle_GameLevel);
		},
		Duration_GameLevel,
		true

	);
	//SpawnEnemy
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnRandom, this, &ATeam_AIGameMode::SpawnEnemyRandom, Duration_SpawnEnemyRandom, false);
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnPatrol, this, &ATeam_AIGameMode::SpawnEnemyPatrol, Duration_SpawnEnemyPatrol, false);
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBoss, this, &ATeam_AIGameMode::SpawnEnemyBoss, Duration_SpawnEnemyBoss, false);
}

TArray<TSubclassOf<ATeam_AIProjectileBase>>& ATeam_AIGameMode::GetAIProjClasses()
{
	return AIClassesProjectiles;
}

int64 ATeam_AIGameMode::WinnerCheck()
{
	int64 aliveCount = 0;
	int64 playerID = 0;
	for (auto playerIter : Players)
	{
		auto player = playerIter.Value;
		if (!player->IsDead())
		{
			aliveCount++;
			playerID = player->PlayerInfo->object_id();
		}
	}
	if (aliveCount == 1)
		return playerID;
	
	return 0;
}

ATeam_AICharacterBase* ATeam_AIGameMode::GetBoss()
{
	return Boss;
}

//void ATeam_AIGameMode::Logout(AController* Exiting)
//{
//	Super::Logout(Exiting);
//	GetNetworkManager()->DisconnectFromGameServer();
//}