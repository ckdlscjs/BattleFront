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
#include "Team_AISpawnPointPlayer.h"
#include "Team_AIMapCamera.h"
#include "Team_AIMagneticField.h"
#include "Components/AudioComponent.h"
#include "ChooseHostWidget.h"
#include "HostScreen.h"
#include "ClientScreen.h"
#include "Team_AIPatrolRoute.h"
#include "Team_AIMagneticField.h"
#include "AbilityDrone.h"
#include "ResultWidget.h"

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
	bAllowTickBeforeBeginPlay = false;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATeam_AISpawnPointRandom::StaticClass(), SpawnPoints_Random);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATeam_AIPatrolRoute::StaticClass(), PatrolRoutes);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATeam_AISpawnPointBoss::StaticClass(), SpawnPoints_Boss);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATeam_AISpawnPointPlayer::StaticClass(), SpawnPoints_Player);
	for (const auto& iter : K2_GetComponentsByClass(UAudioComponent::StaticClass()))
		AudioSystems.Add(TTuple<FString, UAudioComponent*>(iter->GetName(), Cast<UAudioComponent>(iter)));
	auto pc = Cast<ACharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	pc->SetViewTarget(UGameplayStatics::GetActorOfClass(GetWorld(), ATeam_AIMapCamera::StaticClass()));
	
	MagneticField = Cast<ATeam_AIMagneticField>(UGameplayStatics::GetActorOfClass(GetWorld(), ATeam_AIMagneticField::StaticClass()));
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
	chooseOne = CreateWidget<UChooseHostWidget>(GetWorld(), ChooseWidget);
	if (chooseOne)
	{
		chooseOne->AddToViewport();
	}
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

void ATeam_AIGameMode::BeginDestroy()
{
	Super::BeginDestroy();
	if (GetWorld())
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}



void ATeam_AIGameMode::SpawnEnemyRandom()
{
	if (SpawnPoints_Random.Num() <= 0)
		return;
	//TODO : Random Enemy
	//for (const auto& spawnPoint : SpawnPoints_Random)
	for (int i = 0; i < SpawnPoints_Random.Num(); i++)
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
	for (int i = 0; i < PatrolRoutes.Num(); i++)
	{
		//ATeam_AIPatrolRoute* patrolRoute = Cast<ATeam_AIPatrolRoute>(iter);
		ATeam_AIPatrolRoute* patrolRoute = Cast<ATeam_AIPatrolRoute>(PatrolRoutes[i]);
		if (patrolRoute->CheckSpawnPossible())
		{
			for (int j = 0; j < patrolRoute->GetSpawnPoints().Num(); j++)
				//for (const auto& spawnPoint : patrolRoute->GetSpawnPoints())
			{
				/*AddSpawnActor(spawnPoint->SpawnActor());*/
				auto enermy = patrolRoute->GetSpawnPoints()[j]->SpawnActor();
				Enemies.Add(enermy->pos.object_id(), enermy);

				Protocol::C_AISPAWN_PATROL pkt;
				{
					pkt.set_object_id(enermy->pos.object_id());
					pkt.set_container_idx(i);
					pkt.set_pointidx(j);
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

void ATeam_AIGameMode::SetGameLevel(int64 level)
{
	GameLevel = level;
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

	enemy = Cast<ATeam_AIPatrolRoute>(PatrolRoutes[AiSpawnPkt.container_idx()])->GetSpawnPoint(AiSpawnPkt.pointidx())->RecvedSpawnActor();
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

void ATeam_AIGameMode::PlayAudioSystem(FString str)
{
	if (!AudioSystems.Find(str))
		return;
	UAudioComponent* AudioComponent = AudioSystems[str];
	if (!AudioComponent)
		return;
	if (!AudioComponent->Sound)
		return;
	AudioComponent->Play();
}

void ATeam_AIGameMode::StopAudioSystem(FString str, float fadetime)
{
	if (!AudioSystems.Find(str))
		return;
	UAudioComponent* AudioComponent = AudioSystems[str];
	if (!AudioComponent)
		return;
	if (!AudioComponent->Sound)
		return;
	AudioComponent->StopDelayed(fadetime);
}
void ATeam_AIGameMode::SetPlayerCount(const Protocol::S_PLAYERCOUNT& countPkt)
{
	hostScreen = chooseOne->GetHostScreen();
	clientScreen = chooseOne->GetClientScreen();
	if (hostScreen)
		hostScreen->SetPlayerCount(countPkt.playercount());
	if (clientScreen)
		clientScreen->SetPlayerCount(countPkt.playercount());
}

TMap<uint64, AGameCharacter*>& ATeam_AIGameMode::GetPlayers()
{
	return Players;
}

void ATeam_AIGameMode::SetPlayerInfo(bool isMine, const Protocol::PosInfo& Info, uint64 idx)
{

	//FVector location(Info.x(), Info.y(), Info.z());
	//FRotator rotation(Info.pitch(), Info.yaw(), Info.roll());
	//FVector location = SpawnPoints_Player[idx]->GetActorLocation();
	//FRotator rotation = SpawnPoints_Player[idx]->GetActorRotation();
	//AGameCharacter* player = Cast<AGameCharacter>(GetWorld()->SpawnActor(PlayerClass, &location, &rotation));
	AGameCharacter* player = Cast<ATeam_AISpawnPointPlayer>(SpawnPoints_Player[idx])->SpawnActor();
	/*if (!player)
		return;*/
	if (isMine)
	{
		// Case 1
		auto PlayerController = Cast<ACharacterController>(UGameplayStatics::GetPlayerController(this, 0));
		//AGameCharacter* player = Cast<AGameCharacter>(GetWorld()->SpawnActor(PlayerClass, &location, &rotation));
		PlayerController->Possess(player);
		PlayerController->SetCharacter(Cast<AGameCharacter>(PlayerController->GetPawn()));

		//GetWorld()->AddController(temp);
		//temp->Possess(player);
		//temp->SetCharacter(Cast<AGameCharacter>(temp->GetPawn()));

		// Case 2
		//auto PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		//player = Cast<AGameCharacter>(PlayerController->GetPawn());

		MyPlayer = player;
	}
	else
	{
		//if (MyPlayer->PlayerInfo->object_id() != Info.object_id())
		//{
		//	AGameCharacter* player = Cast<AGameCharacter>(GetWorld()->SpawnActor(PlayerClass, &location, &rotation));
		//	Players.Add(Info.object_id(), player);
		//}
	}
	Players.Add(Info.object_id(), player);
	FVector location = player->GetActorLocation();
	FRotator rotation = player->GetActorRotation();
	player->PlayerInfo->set_object_id(Info.object_id());
	player->PlayerInfo->set_x(location.X);
	player->PlayerInfo->set_y(location.Y);
	player->PlayerInfo->set_z(location.Z);
	player->PlayerInfo->set_yaw(rotation.Yaw);
	player->PlayerInfo->set_pitch(rotation.Pitch);
	player->PlayerInfo->set_roll(rotation.Roll);
	player->SetPlayerColor();
	/*if (!player)
		return;*/
		//player->SetPlayerInfo(Info);
		//Players.Add(Info.object_id(), player);
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
	Player->PlayShotSound();
}

void ATeam_AIGameMode::SetPlayerDamaged(const Protocol::S_DAMAGED& dmgPkt)
{
	const uint64 ObjectId = dmgPkt.object_id();

	AGameCharacter** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;
	AGameCharacter* Player = *FindActor;
	Player->UpdateHP(dmgPkt.hp());
	Player->UpdateGuard(dmgPkt.guardpoint());
}

void ATeam_AIGameMode::SetPlayerDead(const Protocol::S_PLAYERDEAD& playerDeadPkt)
{
	const uint64 ObjectId = playerDeadPkt.object_id();
	AGameCharacter** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;
	AGameCharacter* Player = *FindActor;
	Player->DeathEvent();

	Player->SetDead(playerDeadPkt.dead());
}

void ATeam_AIGameMode::SetPlayerExpUP(const Protocol::S_EXPUP& expUpPkt)
{
	const uint64 ObjectId = expUpPkt.ownerid();
	AGameCharacter** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;
	AGameCharacter* Player = *FindActor;

	Player->RecvExpUp(expUpPkt.exp());
}

void ATeam_AIGameMode::SetPlayerLVUP(const Protocol::S_LVUP& lvUpPkt)
{
	const uint64 ObjectId = lvUpPkt.ownerid();
	AGameCharacter** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;
	AGameCharacter* Player = *FindActor;

	Player->RecvLvUP(lvUpPkt.level(), lvUpPkt.currexp());
}

void ATeam_AIGameMode::SetPlayerSKill(const Protocol::S_PLAYERSKILL_BOMB& bombPkt)
{
	const uint64 ObjectId = bombPkt.object_id();
	AGameCharacter** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;
	AGameCharacter* Player = *FindActor;
	if (Player == nullptr)
		return;

	auto am = Player->GetAbilityManager();
	FVector loc = { bombPkt.x(), bombPkt.y(), bombPkt.z() };
	am->RecvMakeBomb(Player, bombPkt.abilityarrayidx(), loc, bombPkt.dmg());
}

void ATeam_AIGameMode::SetPlayerSKill(const Protocol::S_PLAYERSKILL_CHEMICAL& chemPkt)
{
	const uint64 ObjectId = chemPkt.object_id();
	AGameCharacter** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;
	AGameCharacter* Player = *FindActor;
	if (Player == nullptr)
		return;

	auto am = Player->GetAbilityManager();
	FVector loc = { chemPkt.x(), chemPkt.y(), chemPkt.z() };
	am->RecvMakeChemical(Player, chemPkt.abilityarrayidx(), loc, chemPkt.dmg());
}

void ATeam_AIGameMode::SetPlayerSKill(const Protocol::S_PLAYERSKILL_GRANADE& granadePkt)
{
	const uint64 ObjectId = granadePkt.object_id();
	AGameCharacter** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;
	AGameCharacter* Player = *FindActor;
	if (Player == nullptr)
		return;
	auto am = Player->GetAbilityManager();
	FVector loc = { granadePkt.x(), granadePkt.y(), granadePkt.z() };
	FVector rand = { granadePkt.rx(), granadePkt.ry(), granadePkt.rz() };
	am->RecvMakeGranade(Player, granadePkt.abilityarrayidx(), loc, rand, granadePkt.dmg());
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
	auto am = Player->GetAbilityManager();
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
	FRotator rot{ searchDrnPkt.pitch(), searchDrnPkt.yaw(), searchDrnPkt.roll() };
	abilityManager->RecvSearchDrone(loc, rot);
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

void ATeam_AIGameMode::SetPlayerDespawn(uint64 objectID)
{
	auto FindActor = GetPlayers()[objectID];
	if (FindActor == nullptr)
		return;
	GetWorld()->DestroyActor(FindActor);
}

void ATeam_AIGameMode::SetReturnDrone(const Protocol::S_RETURNDRONE& retDrnPkt)
{
	const uint64 ObjectId = retDrnPkt.object_id();
	AGameCharacter** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;
	AGameCharacter* Player = *FindActor;
	auto abilityManager = Player->GetAbilityManager();
	if (abilityManager == nullptr)
		return;
	abilityManager->RecvReturnDrone();
}

void ATeam_AIGameMode::SetAttackDrone(const Protocol::S_ATTACKDRONE& atkDrnPkt)
{
	const uint64 ObjectId = atkDrnPkt.object_id();
	AGameCharacter** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;
	AGameCharacter* Player = *FindActor;
	auto abilityManager = Player->GetAbilityManager();
	if (abilityManager == nullptr)
		return;
	abilityManager->RecvAttackDrone();
}

void ATeam_AIGameMode::SetEatItemMaxHP(const Protocol::S_EATITEM_MAXHPUP& maxHpPkt)
{
	const uint64 ObjectId = maxHpPkt.object_id();
	AGameCharacter** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;
	AGameCharacter* Player = *FindActor;
	Player->RecvCharacterMaxHpUp(maxHpPkt.maxhp());
}

void ATeam_AIGameMode::SetEatItemLVUP(const Protocol::S_EATITEM_LVUP& lvUpPkt)
{
	const uint64 ObjectId = lvUpPkt.object_id();
	AGameCharacter** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;
	AGameCharacter* Player = *FindActor;
	Player->RecvCharacterLevelUp(lvUpPkt.curlv());
}

void ATeam_AIGameMode::SetEatItemDmgUP(const Protocol::S_EATITEM_DMGUP& dmgUpPkt)
{
	const uint64 ObjectId = dmgUpPkt.object_id();
	AGameCharacter** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;
	AGameCharacter* Player = *FindActor;
	Player->RecvCharacterAttackUp(dmgUpPkt.attackdmg());
}

void ATeam_AIGameMode::SetEatItemHealUP(const Protocol::S_EATITEM_HEALHP& healedPkt)
{
	const uint64 ObjectId = healedPkt.object_id();
	AGameCharacter** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;
	AGameCharacter* Player = *FindActor;
	Player->RecvCharacterEatItem(healedPkt.curhp());
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

void ATeam_AIGameMode::SetAIBossAttack2(const Protocol::S_AIATTACK_BOSS2& AIAttackBosspkt)
{
	const uint64 ObjectId = AIAttackBosspkt.object_id();
	if (Boss->pos.object_id() != ObjectId)
		return;
	TArray<AActor*> sumPlayer;
	for (auto pId : AIAttackBosspkt.target_id())
	{
		AGameCharacter** FindActor = Players.Find(pId);
		if (FindActor == nullptr)
			return;
		sumPlayer.Add(*FindActor);
	}
	Boss->SetAttackTargetPlayers(sumPlayer);
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

void ATeam_AIGameMode::SetMagneticField(const Protocol::S_SET_MAGNETICFIELD& magPkt)
{
	FLinearColor rgba{ magPkt.r(), magPkt.g(),magPkt.b(),magPkt.a() };
	if (MagneticField)
		MagneticField->RecvUpdateSafetyFieldValue(rgba, magPkt.radius(), magPkt.time());
}

const class ATeam_AIMagneticField* ATeam_AIGameMode::GetMagneticField() const
{
	return MagneticField;
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
			Protocol::C_WORLD_LVUP wlvPkt;
			{
				wlvPkt.set_worldlevel(GameLevel);
				GetNetworkManager()->SendPacket(wlvPkt);
			}
			if (GameLevel >= GameInstance->GetSizeCharacterDataTable())
				GetWorldTimerManager().PauseTimer(TimerHandle_GameLevel);
		},
		Duration_GameLevel,
		true

	);
	StopAudioSystem(TEXT("BGM_Wait"));
	PlayAudioSystem(TEXT("BGM_Main"));
	//SpawnEnemy
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnRandom, this, &ATeam_AIGameMode::SpawnEnemyRandom, Duration_SpawnEnemyRandom, true);
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnPatrol, this, &ATeam_AIGameMode::SpawnEnemyPatrol, Duration_SpawnEnemyPatrol, true);
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

int64 ATeam_AIGameMode::GetPlayerSpawnPointSize()
{
	return SpawnPoints_Player.Num();
}

void ATeam_AIGameMode::GameStart()
{
	Protocol::C_GAMESTART startPkt;
	GetNetworkManager()->SendPacket(startPkt);

	MagneticField->StartMagnaticField();

	//temp = GetWorld()->SpawnActor<ACharacterController>(controllertemp);
	//temp->SetAsLocalPlayerController();
}

void ATeam_AIGameMode::RemoveStartWidget()
{
	if (hostScreen)
	{
		hostScreen->RemoveFromParent();
		hostScreen = nullptr;
	}
	if (clientScreen)
	{
		clientScreen->RemoveFromParent();
		clientScreen = nullptr;
	}
	chooseOne = nullptr;
}

void ATeam_AIGameMode::ViewGameResult(const Protocol::S_GAMERESULT& pkt)
{
	ResultWidget = CreateWidget<UResultWidget>(GetWorld(), ResultWidgetClass);
	if (ResultWidget)
	{
		if (pkt.object_id() == MyPlayer->PlayerInfo->object_id())
			ResultWidget->SetResultText(true);
		else
			ResultWidget->SetResultText(false);
		ResultWidget->AddToViewport();
	}
}

//void ATeam_AIGameMode::TestStart2(FString str)
//{
//}

//void ATeam_AIGameMode::Logout(AController* Exiting)
//{
//	Super::Logout(Exiting);
//	GetNetworkManager()->DisconnectFromGameServer();
//}