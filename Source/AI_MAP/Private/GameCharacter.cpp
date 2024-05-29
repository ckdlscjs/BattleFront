// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HUDWidget.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Texture2D.h"
#include "../../Team_AI/Public/Team_AIGameInstance.h" // ...?
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Weapon_Pistol.h"
#include "AbilityManager.h"
#include "AbilityBomb.h"
#include "NetworkManager.h"
#include "CharacterAnimInstance.h"
#include "Team_AIGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "AI_MAP.h"

// Sets default values
AGameCharacter::AGameCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);
	
	/*SpawnBound = CreateDefaultSubobject<USphereComponent>(TEXT("Spawn Bound"));
	SpawnBound->SetupAttachment(RootComponent);*/

	RecognizeVisibility = CreateDefaultSubobject<USphereComponent>(TEXT("Recognize Visibility"));
	RecognizeVisibility->SetupAttachment(RootComponent);


	AbilitySpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("AbilitySpawnPoint"));
	AbilitySpawnPoint->SetupAttachment(GetMesh());
	//SpawnPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Spawn Point"));
	//SpawnPoint->SetupAttachment(SpawnBound);

	MaxNum = 5;
	CoolTimeToNormal = 15.f;
	Time = CoolTimeToNormal;

	PlayerInfo = new Protocol::PosInfo();
	DestInfo = new Protocol::PosInfo();
	GuardPoint = 0.f;
}

AGameCharacter::~AGameCharacter()
{
	delete PlayerInfo;
	delete DestInfo;
	PlayerInfo = nullptr;
	DestInfo = nullptr;
}

// Called when the game starts or when spawned
void AGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	auto MyGameMode = Cast<ATeam_AIGameMode>(GetWorld()->GetAuthGameMode());
	AbilityManager = MyGameMode->GetAbilityManager();
	AbilityManager->SetOwner(this);
	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, true);
	AbilityManager->AttachToComponent(AbilitySpawnPoint,Rules);
	PrevCameraRotaion = SpringArmComp->GetComponentRotation();

	if (IsValid(MainHUDWidgetClass))
	{
		MainHUD = Cast<UHUDWidget>(CreateWidget(GetWorld(), MainHUDWidgetClass));
		if (IsValid(MainHUD))
		{
		
			MainHUD->AddToViewport();
		}
	}

	AttachWeapon();
	bDead = false;
	bFight = false;
	

	CharacterStat.Level = 1;
	SetNewLevel(CharacterStat.Level);
}


// Called every frame
void AGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bFight == true)
	{
		Time -= DeltaTime;
		if (Time <= 0)
		{
			bFight = false;
			Time = CoolTimeToNormal;
		}

	}
	AbilityManager->Attack();

		///CheckExp(CharacterStat.CurrExp); // ���� �̺�Ʈ �������� �ٲٰ� �ʹ�...

	//Add Server
	auto NetManager = GetGameInstance()->GetSubsystem<UNetworkManager>();
	//if (NetManager->MyPlayer != this)
	auto gm = Cast<ATeam_AIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gm->GetMyPlayer() != this)
	{
		const Protocol::MoveState State = PlayerInfo->state();

		if (State == Protocol::MOVE_STATE_RUN)
		{
			FVector loc(DestInfo->x(), DestInfo->y(), DestInfo->z());
			FRotator rot(0, DestInfo->yaw(), 0);
			//FVector vel(DestInfo->vx(), DestInfo->vy(), DestInfo->vz());
			//speed = vel.Size();
			//UCharacterAnimInstance* CA = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
			//this->GetVelocity().Size();
			
			//AddMovementInput(vel);
			speed = DestInfo->speed();
			SetActorRotation(rot);
			SetActorLocation(loc);
		}
		else
		{
			speed = 0.0f;
		}
		bReceived = false;
		AimYaw = DestInfo->aimyaw();
	}
}

void AGameCharacter::TakenDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigateBy, AActor* DamageCauser)
{
	if (this == DamageCauser->GetOwner())
	{
		return;
	}
	//FString name = GetActorNameOrLabel();//
	//FString da = FString::SanitizeFloat(Damage);//
	//FString test = name +" " + da;//
	//UKismetSystemLibrary::PrintString(GetWorld(),test
	if (GetNetworkManager()->GameMode->GetMyPlayer()->PlayerInfo->object_id() == 1)
	{
		if (GuardPoint >= 0.f)
		{
			GuardPoint -= Damage;
		}
		else
		{
			CharacterStat.CurrHP -= Damage;
			//TODO : Damaged Packet(Object_id, Updated HP)
			Protocol::C_DAMAGED dmgedPkt;
			{
				dmgedPkt.set_object_id(PlayerInfo->object_id());
				dmgedPkt.set_hp(CharacterStat.CurrHP);

				GetNetworkManager()->SendPacket(dmgedPkt);
			}
		}
		if (CharacterStat.CurrHP <= 0.f)
		{
			//Player Die Event;
			bDead = true;

			// TODO : Dead Packet(Object_id, dead bool)
			Protocol::C_PLAYERDEAD playerDeadPkt;
			{
				playerDeadPkt.set_object_id(PlayerInfo->object_id());
				playerDeadPkt.set_dead(bDead);
				GetNetworkManager()->SendPacket(playerDeadPkt);
			}
			auto gm = Cast<ATeam_AIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			int64 WinnerID = gm->WinnerCheck();
			if (WinnerID)
			{
				// TODO : What object to send?

			}
		}
		
		MainHUD->UpdateHpBar(CharacterStat.MaxHP, CharacterStat.CurrHP);
	}
}

// Called to bind functionality to input
void AGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
}
void AGameCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	OnTakeAnyDamage.AddDynamic(this, &AGameCharacter::TakenDamage);
}
const void AGameCharacter::GetSpringArmRotator(FRotator& Rotator)
{
	Rotator = SpringArmComp->GetComponentRotation();
}

const void AGameCharacter::GetSpringArmLocation(FVector& Location)
{
	Location = SpringArmComp->GetComponentLocation();
}

UWorld* AGameCharacter::GetMyWorld()
{
	return GetWorld();
}

void AGameCharacter::SetSpringArmLocation(const FVector& Location)
{
	SpringArmComp->SetWorldLocation(Location);
}

void AGameCharacter::AddSpringArmRotaion(const FRotator& Rotator)
{
	SpringArmComp->AddWorldRotation(Rotator);
}

void AGameCharacter::SaveSpringArmRotation()
{
	PrevCameraRotaion = SpringArmComp->GetComponentRotation();
}


void AGameCharacter::DetachSpringArm()
{
	FDetachmentTransformRules Rules(EDetachmentRule::KeepWorld,EDetachmentRule::KeepWorld,EDetachmentRule::KeepWorld,true);
	SpringArmComp->DetachFromComponent(Rules);
}

void AGameCharacter::AttachSpringArm()
{
	SpringArmComp->SetWorldLocation(GetActorLocation());
	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget,true);
	SpringArmComp->AttachToComponent(GetCapsuleComponent(), Rules);
}

void AGameCharacter::ResetSpringArmRotation()
{
	SpringArmComp->SetWorldRotation(PrevCameraRotaion);
}

void AGameCharacter::SetNewLevel(int32 NewLevel)
{
	UTeam_AIGameInstance* GameInstance = Cast<UTeam_AIGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance == nullptr)
	{
		return;
	}
	if (GameInstance->GetCharacterRowData(NewLevel) == nullptr)
	{
		return;
	}
	CharacterStat.Level = NewLevel;
	CharacterStat.MaxHP = GameInstance->GetCharacterRowData(NewLevel)->MaxHP;
	CharacterStat.NextExp = GameInstance->GetCharacterRowData(NewLevel)->NextExp;
	if (NewLevel == 1)
	{
		CharacterStat.CurrHP = CharacterStat.MaxHP;
		CharacterStat.CurrExp = 0.f;
		MainHUD->UpdateExpBar(CharacterStat.NextExp, CharacterStat.CurrExp);
	}
	else
	{
		CharacterStat.CurrHP = CharacterStat.CurrHP +50;
	}
	CharacterStat.Attack = GameInstance->GetCharacterRowData(NewLevel)->Attack;
	CharacterStat.Armor = GameInstance->GetCharacterRowData(NewLevel)->Armor;
	CharacterStat.Speed = GameInstance->GetCharacterRowData(NewLevel)->Speed;
	CharacterStat.DropExp = GameInstance->GetCharacterRowData(NewLevel)->DropExp;
	
	MainHUD->UpdateHpBar(CharacterStat.MaxHP, CharacterStat.CurrHP);
	SetRandomTextureArray();
}

const int32 AGameCharacter::GetCharacterLevel()
{
	return CharacterStat.Level;
}


const bool AGameCharacter::IsDead()
{
	return bDead;
}

void AGameCharacter::SetAbility(int32 Index)
{

	bool bFind = AbilityManager->FindAbility(Index);
	int32 FindLevel = 0;
	int32 FindIndex = 0;
	//AbilityManager->SetAbility(Index, bFind, FindIndex, FindLevel);
	if (bFind == false)
	{
		AbilityManager->SetNewAbility();
		/*if (SkillArray.Num() >= 5)
		{
			return;
		}
		AbilityMap.Add(SelectedAbility, SkillIndex);
		FSkillData SkillData;
		SkillData.Index = SkillIndex;
		SkillData.Level = 0;
		SkillArray.Add(SkillData.Index, SkillData.Level);*/
		MainHUD->UpdateSkillImage(AbilityManager->GetSelectedTexture(), AbilityManager->GetSelectedIndex());
		MainHUD->UpdateSkillLevel(AbilityManager->GetSelectedIndex(), 0);
	}
	else if (bFind == true)
	{
		AbilityManager->AbilityLevelUp(FindIndex, FindLevel);
		MainHUD->UpdateSkillLevel(FindIndex, FindLevel);
	}
	AbilityManager->ClearRandomArray();
	MainHUD->HideChooseSkill();
}

void AGameCharacter::AttachWeapon()
{
	//Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass,FVector::ZeroVector, FRotator::ZeroRotator);
	Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, GetActorLocation(), GetActorRotation());
	if (Weapon != nullptr)
	{
		FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepRelative, false);
		Weapon->AttachToComponent(GetMesh(), Rules, WeaponSocket);
		//FVector Loc = Weapon->GetActorLocation();
		//Weapon->SetActorLocation(Loc);
		Weapon->SetOwner(this);
	}	
}

void AGameCharacter::SetFightState(bool IsFight)
{
	bFight = IsFight;
}

void AGameCharacter::SetAimYaw(float Yaw)
{
	AimYaw = Yaw;
}

void AGameCharacter::SetCharacterMovementRotation(bool bState)
{
	GetCharacterMovement()->bOrientRotationToMovement = bState;
}

void AGameCharacter::Fire()
{
	Weapon->Shot();
}

void AGameCharacter::UpdateHpWiget()
{
	MainHUD->UpdateHpBar(CharacterStat.MaxHP, CharacterStat.CurrHP);
}

void AGameCharacter::SetGuardPoint(float Guard)
{
	GuardPoint = Guard;
}

AWeapon* AGameCharacter::GetMyWeapon() const
{
	return Weapon;
}

void AGameCharacter::ExpUp(float Exp)
{
	CharacterStat.CurrExp += Exp;
	if (CharacterStat.NextExp <= CharacterStat.CurrExp)
	{
		CharacterStat.Level++;
		CharacterStat.CurrExp -= CharacterStat.NextExp;
		SetNewLevel(CharacterStat.Level);
	}
	MainHUD->UpdateExpBar(CharacterStat.NextExp, CharacterStat.CurrExp);
}

bool AGameCharacter::FindAbility(int32 Index)
{
	if (Index >= RandomTexture.Num())
	{
		return false;
	}
	SelectedAbility = RandomTexture[Index];
	if (SelectedAbility == nullptr)
	{
		return false;
	}
	if (AbilityMap.Find(SelectedAbility) == nullptr)
	{
		return false;
	}
	return true;
}

const float AGameCharacter::GetDamage()
{
	return CharacterStat.Attack;
}

bool AGameCharacter::IsFight()
{
	return bFight;
}

float AGameCharacter::GetAimYaw()
{
	return AimYaw;
}

float& AGameCharacter::GetCurrentHP()
{
	
	return CharacterStat.CurrHP;
}

float AGameCharacter::GetCurrentMaxHp()
{
	return CharacterStat.MaxHP;
}

void AGameCharacter::SetRandomTextureArray()
{
	MainHUD->ShowChooseSkill();
	AbilityManager->SetRandomIndex();
	/*if (SkillArray.Num() < 5)
	{
		while (RandomTexture.Num() < 3)
		{
			UTexture2D* Rand = nullptr;
			int range = FMath::RandRange(0, (TextureArray.Num() - 1));
			Rand = TextureArray[range];
			RandomTexture.AddUnique(Rand);
		}
	}
	else
	{
		TArray<UTexture2D*> Buff;
		for (auto iter : AbilityMap)
		{
			int32 Level = SkillArray[iter.Value];
			if (Level < 4)
			{
				Buff.AddUnique(iter.Key);
			}
		}
		if (Buff.Num() >=3)
		{
			while (RandomTexture.Num() < 3)
			{
				UTexture2D* Rand = nullptr;
				int range = FMath::RandRange(0, (Buff.Num() - 1));
				Rand = Buff[range];
				RandomTexture.AddUnique(Rand);
			}
		}
		else
		{
			RandomTexture = Buff;
			
		}		
	}*/
	MainHUD->SetChooseSkillImage(AbilityManager->GetRandTextureArray());
}

bool AGameCharacter::IsMyPlayer()
{
	return Cast<AGameCharacter>(this) != nullptr;
}

void AGameCharacter::SetMoveState(Protocol::MoveState State)
{
	if (PlayerInfo->state() == State)
		return;

	PlayerInfo->set_state(State);

	// TODO
}

void AGameCharacter::SetPlayerInfo(const Protocol::PosInfo& Info)
{
	if (PlayerInfo->object_id() != 0)
	{
		assert(PlayerInfo->object_id() == Info.object_id());
	}

	PlayerInfo->CopyFrom(Info);

	FVector Location(Info.x(), Info.y(), Info.z());
	FRotator Rotation(Info.pitch(), Info.yaw(), Info.roll());
	SetActorLocation(Location);
	SetActorRotation(Rotation);
}

void AGameCharacter::SetDestInfo(const Protocol::PosInfo& Info)
{
	if (PlayerInfo->object_id() != 0)
	{
		assert(PlayerInfo->object_id() == Info.object_id());
	}

	DestInfo->CopyFrom(Info);

	SetMoveState(Info.state());
	bReceived = true;
}

AWeapon* AGameCharacter::GetWeapon()
{
	return Weapon;
}

UNetworkManager* AGameCharacter::GetNetworkManager() const
{
	return GetGameInstance()->GetSubsystem<UNetworkManager>();
}

void AGameCharacter::UpdateHP(float hp)
{
	CharacterStat.CurrHP = hp;
	MainHUD->UpdateHpBar(CharacterStat.MaxHP, CharacterStat.CurrHP);
}

void AGameCharacter::SetDead(bool dead)
{
	bDead = dead;
}