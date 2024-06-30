// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityManager.h"
#include "AbilityBase.h"
#include "AbilityGranade.h"
#include "AbilityDrone.h"
#include "AbilityChemical.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "GameCharacter.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Team_AIGameMode.h"

// ToDo : SelectedAbility ������ �� �����

// Sets default values
AAbilityManager::AAbilityManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	RootComponent = SphereComp;
	DroneSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	DroneSpawnPoint->SetupAttachment(RootComponent);
	GuardParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Component"));
	GuardParticle->SetupAttachment(RootComponent);
	SphereComp->SetGenerateOverlapEvents(true);
	bCheckDistance = false;
}

void AAbilityManager::BeginDestroy()
{
	Super::BeginDestroy();
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	}
}

// Called when the game starts or when spawned
void AAbilityManager::BeginPlay()
{
	Super::BeginPlay();
	GuardParticle->bAutoActivate = false;

}

// Called every frame
void AAbilityManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LocalDeltaTime = DeltaTime;
	//test
	{
		auto owner = GetOwner();
		auto player = Cast<AGameCharacter>(owner);
		//if (player)
		//{
		//	player->PlayerInfo->object_id();
		//}
		auto gm = Cast<ATeam_AIGameMode>(GetWorld()->GetAuthGameMode());
		AGameCharacter* myPlayer = gm->GetMyPlayer();
		//if(gm)
		//	myPlayer = gm->GetMyPlayer();

		if (player == myPlayer)
		{
			bool bReturn = CheckDistanceToDrone();
			DroneReturn();
		}
	}
}

void AAbilityManager::Attack()
{
	for (auto AbilityData : AbilityClassArray)
	{
		//AbilityData.GetDefaultObject()->SetOwner(GetOwner());
		AbilityType Type = AbilityData.GetDefaultObject()->GetType();
		if (AbilityData.GetDefaultObject()->CheckTime(GetWorld()->GetDeltaSeconds()))
		{
			int idx;
			for (idx = 0; idx < DefaultAbilityArray.Num(); idx++)
			{
				if (AbilityData == DefaultAbilityArray[idx])
					break;
			}

			if (Type == AbilityType::Range)
			{
				int32 Num = AbilityData.GetDefaultObject()->GetProjCount();
				for (int i = 0; i < Num; i++)
				{
					FRotator Rot = FRotator::ZeroRotator;
					FVector Loc = GetActorLocation();
					AAbilityBase* AttackObject = GetWorld()->SpawnActor<AAbilityBase>(AbilityData, Loc, FRotator::ZeroRotator);
					AttackObject->SetOwner(GetOwner());
					AttackObject->SetLocation(Loc, idx);
				}
			}
			else if (Type == AbilityType::Guard)
			{
				AbilityData.GetDefaultObject()->ChargeGuardPoint();
				AGameCharacter* MyOwner = Cast<AGameCharacter>(GetOwner());
				float GuardPoint = AbilityData.GetDefaultObject()->GetAbilityDetail();
				MyOwner->SetGuardPoint(GuardPoint);
				if (GuardSound)
				{
					UGameplayStatics::PlaySoundAtLocation(this, GuardSound, GetActorLocation());
				}
				if (GuardParticle->Template)
				{

					GuardParticle->SetWorldLocation(GuardParticle->GetComponentLocation());
					GuardParticle->Activate(true);
				}
				Protocol::C_PLAYERSKILL_GUARD guardPkt;
				{
					guardPkt.set_object_id(MyOwner->PlayerInfo->object_id());
					guardPkt.set_guardpoint(GuardPoint);

					GetNetworkManager()->SendPacket(guardPkt);
				}
			}
		}
	}
}

void AAbilityManager::SetRandomIndex()
{
	if (AbilityClassArray.Num() < 4)
	{
		while (RandTextureArray.Num() < 3)
		{
			TSubclassOf<AAbilityBase> Temp = nullptr;
			int range = FMath::RandRange(0, DefaultAbilityArray.Num() - 1);
			Temp = DefaultAbilityArray[range];
			if (CheckAbilityLevel(Temp))
			{
				RandomAbilityArray.AddUnique(Temp);
				RandTextureArray.AddUnique(Temp.GetDefaultObject()->GetTexture());
			}
		}
	}
	else
	{
		TArray<TSubclassOf<AAbilityBase>> Buff;
		TArray<UTexture2D*> Temp;
		for (auto AbilityData : AbilityClassArray)
		{
			int32 Level = AbilityData.GetDefaultObject()->GetAbilityLevel();
			if (Level < 4)
			{
				Buff.AddUnique(AbilityData);
				Temp.AddUnique(AbilityData.GetDefaultObject()->GetTexture());
			}
		}
		if (Buff.Num() >= 3)
		{
			while (RandTextureArray.Num() < 3)
			{
				UTexture2D* rand = nullptr;
				int range = FMath::RandRange(0, (Buff.Num() - 1));
				rand = Temp[range];
				RandomAbilityArray.AddUnique(Buff[range]);
				RandTextureArray.AddUnique(rand);
			}
		}
		else
		{
			RandTextureArray = Temp;
			RandomAbilityArray = Buff;
		}
	}
}

void AAbilityManager::SetNewAbility()
{
	if (AbilityClassArray.Num() > 4)
	{
		return;
	}
	AbilityClassArray.Add(SelectedAbility);

	int i; 
	for (i = 0; i < DefaultAbilityArray.Num(); i++)
	{
		if (SelectedAbility == DefaultAbilityArray[i])
			break;
	}

	AbilityType Type = SelectedAbility.GetDefaultObject()->GetType();

	if (Type == AbilityType::Drone)
	{
		DroneActor = GetWorld()->SpawnActor<AAbilityBase>(SelectedAbility, DroneSpawnPoint->GetComponentLocation(), FRotator::ZeroRotator);
		DroneActor->SetOwner(GetOwner());
		DroneActor->SetDepthStencil();
		if (DroneSpawnSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DroneSpawnSound, GetActorLocation());
		}
		FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::SnapToTarget, true);
		DroneActor->AttachToComponent(DroneSpawnPoint, Rules);
		DroneActor->SetAttachedState(true);
		GetWorldTimerManager().SetTimer(DroneAttackTimer, this, &AAbilityManager::DroneAttack, 3.f, true, 1);

		// TODO : Make Drone
		// need owner->object_id, abilityArrayIdx

		Protocol::C_MAKEDRONE makeDronePkt;
		{
			auto Player = Cast<AGameCharacter>(GetOwner());
			makeDronePkt.set_object_id(Player->PlayerInfo->object_id());
			makeDronePkt.set_abilityarrayidx(i);
			GetNetworkManager()->SendPacket(makeDronePkt);
		}
	}
	else if (Type == AbilityType::Heal)
	{
		HealComponent = GetWorld()->SpawnActor<AAbilityBase>(SelectedAbility, GetActorLocation(), FRotator::ZeroRotator);
		HealComponent->SetOwner(this);
		FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, true);
		HealComponent->AttachToActor(this, Rules);
		GetWorldTimerManager().SetTimer(HealTimer, this, &AAbilityManager::HealAbility, 10.f, true, 1);

		Protocol::C_PLAYERSKILL_HEAL healPkt;
		{
			AGameCharacter* MyOwner = Cast<AGameCharacter>(GetOwner());
			healPkt.set_object_id(MyOwner->PlayerInfo->object_id());
			healPkt.set_abilityarrayidx(i);

			GetNetworkManager()->SendPacket(healPkt);
		}
	}
}

void AAbilityManager::AbilityLevelUp(int32& Index, int32& Level)
{
	if (AbilityClassArray.Find(SelectedAbility) != INDEX_NONE)
	{
		Index = AbilityClassArray.Find(SelectedAbility);
		SelectedAbility.GetDefaultObject()->AbilityLevelUp();
		Level = SelectedAbility.GetDefaultObject()->GetAbilityLevel();
		if (SelectedAbility.GetDefaultObject()->GetType() == AbilityType::Guard)
		{
			AGameCharacter* player = Cast<AGameCharacter>(GetOwner());
			player->UpdateGuradUI(SelectedAbility.GetDefaultObject()->GetAbilityDetail());
		}
	} 
}

bool AAbilityManager::FindAbility(int32 Index)
{
	if (Index >= RandTextureArray.Num())
	{
		return false;
	}

	SelectedAbility = RandomAbilityArray[Index];
	if (SelectedAbility == nullptr)
	{
		return false;
	}
	if (AbilityClassArray.Find(SelectedAbility) == INDEX_NONE)
	{
		return false;
	}
	return true;
}

TArray<class UTexture2D*>& AAbilityManager::GetRandTextureArray()
{
	return RandTextureArray;
}

void AAbilityManager::ClearRandomArray()
{
	RandTextureArray.Empty();
	RandomAbilityArray.Empty();
}

UTexture2D* AAbilityManager::GetSelectedTexture()
{
	if (SelectedAbility == nullptr)
	{
		return nullptr;
	}
	return SelectedAbility.GetDefaultObject()->GetTexture();
}

int32 AAbilityManager::GetSelectedIndex()
{
	return (AbilityClassArray.Num() - 1);
}

// 이거 attack이 아니라. 드론이 탐색한다고 생각하는게 편할꺼같은데...?
void AAbilityManager::DroneAttack()
{
	bool AttackEnd = false;
	FVector Loc = GetActorLocation();
	if (DroneActor->GetAttachedState())
	{
		FDetachmentTransformRules Rules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);
		DroneActor->SetAttachedState(false);
		DroneActor->DetachFromActor(Rules);
	}
	DroneActor->SetLocation(Loc);
	DroneActor->SetDroneRotation();
	//TODO : DroneAttackPkt;
	// need : owner_id

	Protocol::C_SEARCHDRONE searchDronePkt;
	{
		FRotator rot = DroneActor->GetActorRotation();
		auto player = Cast<AGameCharacter>(GetOwner());
		searchDronePkt.set_object_id(player->PlayerInfo->object_id());
		searchDronePkt.set_x(Loc.X);
		searchDronePkt.set_y(Loc.Y);
		searchDronePkt.set_z(Loc.Z);
		searchDronePkt.set_yaw(rot.Yaw);
		searchDronePkt.set_pitch(rot.Pitch);
		searchDronePkt.set_roll(rot.Roll);
	}
	GetNetworkManager()->SendPacket(searchDronePkt);
}

void AAbilityManager::DroneReturn()
{
	if (DroneActor != nullptr)
	{
		if (DroneActor->GetDroneState() == DroneState::Return)
		{
			GetWorldTimerManager().PauseTimer(DroneAttackTimer);
			DroneActor->SetDroneNoneState();
			FVector loc = GetDronePointLocation();
			FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
			DroneActor->AttachToComponent(DroneSpawnPoint, Rules);
			DroneActor->SetAttachedState(true);
			GetWorldTimerManager().UnPauseTimer(DroneAttackTimer);

			//TODO : ReturnDronPkt
			// OwnerID
			Protocol::C_RETURNDRONE retDrnPkt;
			{
				auto player = Cast<AGameCharacter>(GetOwner());
				retDrnPkt.set_object_id(player->PlayerInfo->object_id());
			}
			GetNetworkManager()->SendPacket(retDrnPkt);
		}
	}
}

bool AAbilityManager::CheckDistanceToDrone()
{
	if (DroneActor != nullptr)
	{
		FVector DroneLoc = DroneActor->GetActorLocation();
		FVector MyLoc = GetActorLocation();
		float Distance = (MyLoc - DroneLoc).Size();
		if (Distance > 500.f)
		{
			DroneTimer += LocalDeltaTime;
			if (DroneTimer >= 1.25f)
			{
				if (DroneActor->GetDroneState() != DroneState::Return)
				{
					DroneActor->SetDroneStateReturn();
				}
			}
			return true;
		}
		else
		{
			DroneTimer = 0.f;
		}
	}
	return false;
}

void AAbilityManager::ResetSelectedAbility()
{
	SelectedAbility = nullptr;
}

void AAbilityManager::SetCharacterGuard()
{
	AGameCharacter* MyOwner = Cast<AGameCharacter>(GetOwner());
	float GuardPoint = SelectedAbility.GetDefaultObject()->GetAbilityDetail();
	MyOwner->SetGuardPoint(GuardPoint);
}

FVector AAbilityManager::GetDronePointLocation()
{
	return DroneSpawnPoint->GetComponentLocation();
}

void AAbilityManager::HealAbility()
{
	AGameCharacter* MyOwner = Cast<AGameCharacter>(GetOwner());
	HealComponent->HealCharacterHp(MyOwner->GetCurrentHP(), MyOwner->GetCurrentMaxHp());
	MyOwner->UpdateHpWiget();
	MyOwner->SpawnHealEffect();
	Protocol::C_PLAYERHEAL updatedhealPkt;
	{
		updatedhealPkt.set_object_id(MyOwner->PlayerInfo->object_id());
		updatedhealPkt.set_updeatedhp(MyOwner->GetCurrentHP());

		GetNetworkManager()->SendPacket(updatedhealPkt);
	}
}

void AAbilityManager::StopAbility()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	}
	if (DroneActor != nullptr)
	{
		DroneActor->Destroy();
	}
}

void AAbilityManager::SetVisibility(bool visible)
{
	if (DroneActor != nullptr)
	{
		DroneActor->SetVisibility(visible);
	}
}


bool AAbilityManager::CheckAbilityLevel(TSubclassOf<AAbilityBase> Base)
{
	if (AbilityClassArray.Find(Base) == INDEX_NONE)
	{
		return true;
	}
	else
	{
		int32 Level = Base.GetDefaultObject()->GetAbilityLevel();
		if (Level < 4)
		{
			return true;
		}
	}
	return false;
}

UNetworkManager* AAbilityManager::GetNetworkManager() const
{
	return GetGameInstance()->GetSubsystem<UNetworkManager>();
}

void AAbilityManager::RecvMakeBomb(AGameCharacter* owner, int abilityIdx, FVector loc, float dmg)
{
	FRotator Rot = FRotator::ZeroRotator;
	AAbilityBase* AttackObject = GetWorld()->SpawnActor<AAbilityBase>(DefaultAbilityArray[abilityIdx], loc, FRotator::ZeroRotator);
	AttackObject->SetOwner(owner);
	AttackObject->SetAbilityDetail(dmg);
}

void AAbilityManager::RecvMakeChemical(AGameCharacter* owner, int abilityIdx, FVector loc, float dmg)
{
	FRotator Rot = FRotator::ZeroRotator;
	AAbilityBase* AttackObject = GetWorld()->SpawnActor<AAbilityBase>(DefaultAbilityArray[abilityIdx], loc, FRotator::ZeroRotator);
	AttackObject->SetOwner(owner);
	AttackObject->SetAbilityDetail(dmg);
	Cast<AAbilityChemical>(AttackObject)->PlayChemicalSound();
}

void AAbilityManager::RecvMakeGranade(AGameCharacter* owner, int abilityIdx, FVector loc, FVector rand, float dmg)
{
	FRotator Rot = FRotator::ZeroRotator;
	AAbilityBase* AttackObject = GetWorld()->SpawnActor<AAbilityBase>(DefaultAbilityArray[abilityIdx], loc, FRotator::ZeroRotator);

	AttackObject->SetOwner(owner);
	Cast<AAbilityGranade>(AttackObject)->SetForce(rand);
	AttackObject->SetAbilityDetail(dmg);
}

void AAbilityManager::RecvMakeHeal(int abilityIdx)
{
	HealComponent = GetWorld()->SpawnActor<AAbilityBase>(DefaultAbilityArray[abilityIdx], GetActorLocation(), FRotator::ZeroRotator);
	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, true);
	HealComponent->AttachToActor(this, Rules);
}

void AAbilityManager::RecvUpdateHP(int hp)
{
}

AAbilityBase* AAbilityManager::GetDrone()
{
	return DroneActor;
}

void AAbilityManager::RecvMakeDrone(int64 idx, AGameCharacter* owner)
{
	DroneActor = GetWorld()->SpawnActor<AAbilityBase>(DefaultAbilityArray[idx], DroneSpawnPoint->GetComponentLocation(), FRotator::ZeroRotator);
	DroneActor->SetOwner(owner);
	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, true);
	DroneActor->AttachToComponent(DroneSpawnPoint, Rules);
	DroneActor->SetAttachedState(true);
	if (DroneSpawnSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DroneSpawnSound, GetActorLocation());
	}
}

void AAbilityManager::RecvSearchDrone(FVector loc, FRotator rot)
{
	if (DroneActor->GetAttachedState())
	{
		FDetachmentTransformRules Rules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);
		DroneActor->SetAttachedState(false);
		DroneActor->DetachFromActor(Rules);
	}
	DroneActor->SetLocation(loc);
	DroneActor->SetActorRotation(rot);
	Cast<AAbilityDrone>(DroneActor)->PlayDroneMoveSound();
}

void AAbilityManager::RecvMoveDrone(FVector loc)
{
	DroneActor->SetActorLocation(loc);
}

void AAbilityManager::RecvReturnDrone()
{
	//FVector loc = GetDronePointLocation();
	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, true);
	DroneActor->AttachToComponent(DroneSpawnPoint, Rules);
	DroneActor->SetAttachedState(true);
}

void AAbilityManager::RecvAttackDrone()
{
	if (DroneActor)
	{
		Cast<AAbilityDrone>(DroneActor)->ActivateAttakPaticle();
		Cast<AAbilityDrone>(DroneActor)->PlayDroneAttackSound();
	}

}
