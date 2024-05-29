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
	Duration_SpawnEnemyRandom = 7.0f;
	Duration_SpawnEnemyPatrol = 10.0f;
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
		
		Protocol::C_AISPAWNRANDOM pkt;
		{
			pkt.set_object_id(enermy->pos.object_id());
			FVector loc(enermy->GetActorLocation());
			pkt.set_x(loc.X);
			pkt.set_y(loc.Y);
			pkt.set_z(loc.Z);
			FRotator rot(enermy->GetActorRotation());
			pkt.set_yaw(rot.Yaw);
			pkt.set_pitch(rot.Pitch);
			pkt.set_roll(rot.Roll);
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

				Protocol::C_AISPAWNRANDOM pkt;
				{
					pkt.set_object_id(enermy->pos.object_id());
					FVector loc(enermy->GetActorLocation());
					pkt.set_x(loc.X);
					pkt.set_y(loc.Y);
					pkt.set_z(loc.Z);
					FRotator rot(enermy->GetActorRotation());
					pkt.set_yaw(rot.Yaw);
					pkt.set_pitch(rot.Pitch);
					pkt.set_roll(rot.Roll);
					pkt.set_container_idx(i);
					GetNetworkManager()->SendPacket(pkt);
				}
			}
		}
	}
}

int32 ATeam_AIGameMode::GetGameLevel() const
{
	return GameLevel;
}

void ATeam_AIGameMode::AddSpawnActor(AActor* actor)
{
	//if (!actor)
	//	return;
	//Enemies.Add(actor);
}

void ATeam_AIGameMode::DeleteSpawnActor(AActor* actor)
{
	/*if (!actor)
		return;
	Enemies.Remove(actor);*/
}

void ATeam_AIGameMode::AddSpawnActor(const Protocol::ObjectInfo& AiInfo)
{
	ATeam_AICharacterBase* enemy = nullptr;
	FVector location(AiInfo.pos_info().x(), AiInfo.pos_info().y(), AiInfo.pos_info().z());
	FRotator rotation(AiInfo.pos_info().pitch(), AiInfo.pos_info().yaw(), AiInfo.pos_info().roll());
	//if (MyPlayer->PlayerInfo->object_id() == 1)
	//{
	//	//changeClasses(tarray)
	//	//enemy = Cast<ATeam_AICharacterBase>(GetWorld()->SpawnActor(AIClass, &location, &rotation));
	//	enemy = Cast<ATeam_AICharacterBase>(GetWorld()->SpawnActor(AIClass, &location));
	//	enemy->pos = AiInfo.pos_info();
	//}
	//else
	//{
		//changeClasses(tarray)
		//enemy = Cast<ATeam_AICharacter_Recv>(GetWorld()->SpawnActor(AIClassRecv, &location, &rotation));
		enemy = Cast<ATeam_AICharacter_Recv>(GetWorld()->SpawnActor(AIClassRecv, &location));
	//}	
	Enemies.Add(AiInfo.object_id(), enemy);
}

void ATeam_AIGameMode::AddSpawnActor(const Protocol::S_AISPAWNRANDOM& AiRandomSpawnPkt)
{
	ATeam_AICharacter_Recv* enemy = nullptr;

	enemy = Cast<ATeam_AISpawnPointRandom>(SpawnPoints_Random[AiRandomSpawnPkt.container_idx()])->RecvedSpawnActor();
	enemy->pos.set_object_id(AiRandomSpawnPkt.object_id());
	Enemies.Add(AiRandomSpawnPkt.object_id(), enemy);
}


void ATeam_AIGameMode::DeleteSpawnActor(const Protocol::ObjectInfo& AiInfo)
{
	
}

float ATeam_AIGameMode::GetDurationSpawnEnemyRandom() const
{
	return Duration_SpawnEnemyRandom;
}

float ATeam_AIGameMode::GetDurationSpawnEnemyPatrol() const
{
	return Duration_SpawnEnemyPatrol;
}

void ATeam_AIGameMode::SpawnAbilityManager()
{
	AbilityManager = GetWorld()->SpawnActor<AAbilityManager>(AbilityManagerClass, FVector::ZeroVector, FRotator::ZeroRotator);
}

AAbilityManager* ATeam_AIGameMode::GetAbilityManager()
{

	if (AbilityManager == nullptr)
	{
		SpawnAbilityManager();
	}
	return AbilityManager;
}

const TMap<uint64, AGameCharacter*>& ATeam_AIGameMode::GetPlayers() const
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
		auto PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		player = Cast<AGameCharacter>(PlayerController->GetPawn());
		MyPlayer = player;
	}
	else
	{
		if(MyPlayer->PlayerInfo->object_id() != Info.object_id())	// Test
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
	enemy->RecvAttack();
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
