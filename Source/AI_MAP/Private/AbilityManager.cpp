// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityManager.h"
#include "AbilityBase.h"
#include "AbilityBomb.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "GameCharacter.h"

// ToDo : SelectedAbility 선택한 후 지우기

// Sets default values
AAbilityManager::AAbilityManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	RootComponent = SphereComp;
	DroneSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	DroneSpawnPoint->SetupAttachment(RootComponent);
	SphereComp->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AAbilityManager::BeginPlay()
{
	Super::BeginPlay();
	//GetWorldTimerManager().SetTimer(Timer, this, &ATeam_AIGameMode::SpawnEnemyRandom, Duration_SpawnEnemyRandom, true);

}

// Called every frame
void AAbilityManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CheckDistanceToDrone())
	{
		GetWorldTimerManager().PauseTimer(DroneAttackTimer);
		ReturnDrone();
		GetWorldTimerManager().UnPauseTimer(DroneAttackTimer);
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
			if (Type == AbilityType::Range)
			{
				int32 Num = AbilityData.GetDefaultObject()->GetProjCount();
				int32 Lv = AbilityData.GetDefaultObject()->GetAbilityLevel()+1;
				for (int i = 0; i < Num*Lv; i++)
				{
					FRotator Rot = FRotator::ZeroRotator;
					FVector Loc = GetActorLocation();
					AAbilityBase* AttackObject = GetWorld()->SpawnActor<AAbilityBase>(AbilityData,Loc, FRotator::ZeroRotator);
					AttackObject->SetOwner(GetOwner());
					AttackObject->SetLocation(Loc);
				}				
			}
			else if (Type == AbilityType::Guard)
			{
				AbilityData.GetDefaultObject()->ChargeGuardPoint();
				AGameCharacter* MyOwner = Cast<AGameCharacter>(GetOwner());
				float GuardPoint = AbilityData.GetDefaultObject()->GetAbilityDetail();
				MyOwner->SetGuardPoint(GuardPoint);
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
	
	AbilityType Type = SelectedAbility.GetDefaultObject()->GetType();

	if (Type == AbilityType::Drone)
	{
		DroneActor = GetWorld()->SpawnActor<AAbilityBase>(SelectedAbility,DroneSpawnPoint->GetComponentLocation(), FRotator::ZeroRotator);
		DroneActor->SetOwner(GetOwner());
		FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, true);
		DroneActor->AttachToComponent(DroneSpawnPoint, Rules);
		GetWorldTimerManager().SetTimer(DroneAttackTimer,this,&AAbilityManager::DroneAttack, 3.f, true, 1);
	}
	else if (Type == AbilityType::Heal)
	{
		HealComponent = GetWorld()->SpawnActor<AAbilityBase>(SelectedAbility, GetActorLocation(), FRotator::ZeroRotator);
		HealComponent->SetOwner(this);
		FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, true);
		HealComponent->AttachToActor(this, Rules);
		GetWorldTimerManager().SetTimer(HealTimer, this, &AAbilityManager::HealAbility, 10.f, true, 1);
	}
}

void AAbilityManager::AbilityLevelUp(int32& Index, int32& Level)
{
	if (AbilityClassArray.Find(SelectedAbility) != INDEX_NONE)
	{
		Index = AbilityClassArray.Find(SelectedAbility);
		SelectedAbility.GetDefaultObject()->AbilityLevelUp();
		Level = SelectedAbility.GetDefaultObject()->GetAbilityLevel();
		FString Name = SelectedAbility.GetDefaultObject()->GetAbilityName();
		for (auto Base : AbilityArray)
		{
			if (Base.Key->GetAbilityName() == Name)
			{
				Base.Value++;
				break;
			}
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
	return SelectedAbility.GetDefaultObject()->GetTexture();
}

int32 AAbilityManager::GetSelectedIndex()
{
	int32 FindLevel = SelectedAbility.GetDefaultObject()->GetAbilityLevel();
	return (AbilityClassArray.Num() - 1);
}

void AAbilityManager::DroneAttack()
{
	bool AttackEnd = false;
	FVector Loc = GetActorLocation();
	FDetachmentTransformRules Rules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld,false);
	DroneActor->DetachFromActor(Rules);
	DroneActor->SetLocation(Loc);

}

bool AAbilityManager::CheckDistanceToDrone()
{
	if (DroneActor != nullptr)
	{
		FVector DroneLoc = DroneActor->GetActorLocation();
		FVector MyLoc = GetActorLocation();
		float Distance = (MyLoc - DroneLoc).Size();
		if (Distance >= 500.f)
		{
			return true;
		}
	}
	return false;
}

void AAbilityManager::ReturnDrone()
{
	FVector Location = GetDronePointLocation();
	DroneActor->SetActorLocation(Location);
	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, true);
	DroneActor->AttachToComponent(DroneSpawnPoint, Rules);
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