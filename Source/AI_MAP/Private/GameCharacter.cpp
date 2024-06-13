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
#include "CharacterController.h"
#include "HealthBar.h"
#include "Components/WidgetComponent.h"
#include "PaperSpriteComponent.h"
#include "DamageTextActor.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/SceneCapture2D.h"
#include "Components/SceneCaptureComponent2D.h"
// Sets default values
AGameCharacter::AGameCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);


	RecognizeVisibility = CreateDefaultSubobject<USphereComponent>(TEXT("Recognize Visibility"));
	RecognizeVisibility->SetupAttachment(RootComponent);


	AbilitySpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("AbilitySpawnPoint"));
	AbilitySpawnPoint->SetupAttachment(RootComponent);
	
	DamageTextPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Damage Spawn Point"));
	DamageTextPoint->SetupAttachment(RootComponent);

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(RootComponent);

	LevelUpParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Component"));
	LevelUpParticleSystemComponent->SetupAttachment(RootComponent);
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	SceneCaptureComponent->SetupAttachment(RootComponent);

	MaxNum = 5;
	CoolTimeToNormal = 15.f;
	Time = CoolTimeToNormal;
	bIsLevelUp = false;
	LevelUpCount = 0;
	PlayerInfo = new Protocol::PosInfo();
	DestInfo = new Protocol::PosInfo();
	MaxGuardPoint = 0.f;
	CurrentGuardPoint = 0.f;
	bChangeColor = false;

	PaperSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("IconSprite"));
	PaperSprite->SetupAttachment(RootComponent);
	PaperSprite->SetRelativeLocation({ 0, 0, 2000 });
	PaperSprite->SetRelativeRotation({ 0, 90, -90 });
	PaperSprite->SetRelativeScale3D({ 20, 20, 20 });
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
	/*APlayerController* MyController = Cast<APlayerController>(GetController());
	if (MyController == nullptr)
	{
		SceneCaptureComponent->Deactivate();

	}*/
	if (HealthBarWidgetClass != nullptr)
	{
		HealthBarWidget->SetWidgetClass(HealthBarWidgetClass);
		HealthBar = Cast<UHealthBar>(HealthBarWidget->GetUserWidgetObject());
	}


	AbilityManager = GetWorld()->SpawnActor<AAbilityManager>(AbilityManagerClass, AbilitySpawnPoint->GetComponentLocation(), FRotator::ZeroRotator);
	if (AbilityManager->GetOwner() == nullptr)
	{
		AbilityManager->SetOwner(this);
		FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::SnapToTarget, true);
		AbilityManager->AttachToComponent(AbilitySpawnPoint, Rules);
	}
	PrevCameraRotaion = SpringArmComp->GetComponentRotation();



	AttachWeapon();
	bDead = false;
	bFight = false;

	CharacterStat.Level = 1;
	LevelUpParticleSystemComponent->bAutoActivate = false;
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
	auto gm = Cast<ATeam_AIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (bChangeColor == false)
	{
		if (gm->GetMyPlayer() == this)
		{
			HealthBar->SetColorHpBar();
		}
		bChangeColor = true;
	}
	
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
			FVector loc(DestInfo->x(), DestInfo->y(), DestInfo->z());
			FRotator rot(0, DestInfo->yaw(), 0);
			SetActorRotation(rot);
			SetActorLocation(loc);
			//test
		}
		bReceived = false;
		AimYaw = DestInfo->aimyaw();
	}
}

void AGameCharacter::SetVisibility(bool visible)
{
	GetMesh()->SetVisibility(visible);
	Weapon->SetVisibility(visible);
}

void AGameCharacter::CharacterMaxHpUp(float value)
{
	CharacterStat.MaxHP += value;
	if (MainHUD != nullptr)
	{
		MainHUD->UpdateHpBar(CharacterStat.MaxHP, CharacterStat.CurrHP);
	}
	if (HealthBar != nullptr)
	{
		HealthBar->SetPercentage(CharacterStat.MaxHP, CharacterStat.CurrHP);
	}
}

void AGameCharacter::CharacterLevelUp()
{
	CharacterStat.Level++;
	SetNewLevel(CharacterStat.Level);
}

void AGameCharacter::CharacterAttackUp(float value)
{
	CharacterStat.Attack += value;
}

void AGameCharacter::CharacterEatItem(float value)
{
	CharacterStat.CurrHP += value;
	if (CharacterStat.CurrHP > CharacterStat.MaxHP)
	{
		CharacterStat.CurrHP = CharacterStat.MaxHP;
	}
	if (MainHUD)
		MainHUD->UpdateHpBar(CharacterStat.MaxHP, CharacterStat.CurrHP);
}

void AGameCharacter::TakenDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigateBy, AActor* DamageCauser)
{

	if (this == DamageCauser->GetOwner())
	{
		return;
	}
	if (bDead == true)
	{
		return;
	}
	if (GetNetworkManager()->GameMode->GetMyPlayer()->PlayerInfo->object_id() == 1)
	{
		if (CurrentGuardPoint > 0.f)
		{
			float Remain = 0.f;
			if (Damage > CurrentGuardPoint)
			{
				Remain = Damage - CurrentGuardPoint;
				CurrentGuardPoint = 0.f;
				CharacterStat.CurrHP -= Remain;
			}
			else
			{
				CurrentGuardPoint -= Damage;
			}
			if (HealthBar != nullptr)
			{
				HealthBar->SetGuardPercent(MaxGuardPoint, CurrentGuardPoint);
			}
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
			CharacterStat.CurrHP = 0.f;
			//Player Die Event;
			
			DeathEvent();
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
		if (MainHUD)
		{
			MainHUD->UpdateHpBar(CharacterStat.MaxHP, CharacterStat.CurrHP);
			MainHUD->UpdateGuardPoint(CurrentGuardPoint, MaxGuardPoint);
		}
		if (HealthBar != nullptr)
		{
			HealthBar->SetPercentage(CharacterStat.MaxHP, CharacterStat.CurrHP);
		}
		CreateDamageWidget(Damage);
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

void AGameCharacter::CreateDamageWidget(float DamageAmount)
{
	DamageTextActor = GetWorld()->SpawnActor<ADamageTextActor>(DamageTextClass, DamageTextPoint->GetComponentLocation(), FRotator::ZeroRotator);
	DamageTextActor->SpawnDamageText(DamageAmount);
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
	FDetachmentTransformRules Rules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, true);
	SpringArmComp->DetachFromComponent(Rules);
}

void AGameCharacter::AttachSpringArm()
{
	SpringArmComp->SetWorldLocation(GetActorLocation());
	FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, true);
	SpringArmComp->AttachToComponent(GetCapsuleComponent(), Rules);
}

void AGameCharacter::ResetSpringArmRotation()
{
	SpringArmComp->SetWorldRotation(PrevCameraRotaion);
}

void AGameCharacter::SetNewLevel(int32 NewLevel)
{
	if (LevelUpParticleSystemComponent->Template)
	{
		
		LevelUpParticleSystemComponent->SetWorldLocation(LevelUpParticleSystemComponent->GetComponentLocation());
		LevelUpParticleSystemComponent->Activate(true);
	}

	UTeam_AIGameInstance* GameInstance = Cast<UTeam_AIGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance == nullptr)
	{
		return;
	}
	if (GameInstance->GetCharacterRowData(NewLevel) == nullptr)
	{
		return;
	}
	if (bDead == true)
	{
		return;
	}
	bIsLevelUp = true;
	LevelUpCount++;
	CharacterStat.Level = NewLevel;
	int32 TempLevel = NewLevel - 1;
	float HpAmount = 0.f;
	float ExpAmount = 0.f;
	float AttackAmount = 0.f;
	float ArmorAmount = 0.f;
	float SpeedAmount = 0.f;
	float DropExpAmount = 0.f;
	if (NewLevel > 1 )
	{
		HpAmount = GameInstance->GetCharacterRowData(NewLevel)->MaxHP - GameInstance->GetCharacterRowData(TempLevel)->MaxHP;
		ExpAmount = GameInstance->GetCharacterRowData(NewLevel)->NextExp - GameInstance->GetCharacterRowData(TempLevel)->NextExp;
		AttackAmount = GameInstance->GetCharacterRowData(NewLevel)->Attack - GameInstance->GetCharacterRowData(TempLevel)->Attack;
		ArmorAmount = GameInstance->GetCharacterRowData(NewLevel)->Armor - GameInstance->GetCharacterRowData(TempLevel)->Armor;
		SpeedAmount = GameInstance->GetCharacterRowData(NewLevel)->Speed - GameInstance->GetCharacterRowData(TempLevel)->Speed;
		DropExpAmount = GameInstance->GetCharacterRowData(NewLevel)->DropExp - GameInstance->GetCharacterRowData(TempLevel)->DropExp;
	}
	else
	{
		HpAmount = GameInstance->GetCharacterRowData(NewLevel)->MaxHP;
		ExpAmount = GameInstance->GetCharacterRowData(NewLevel)->NextExp;
		AttackAmount = GameInstance->GetCharacterRowData(NewLevel)->Attack;
		ArmorAmount = GameInstance->GetCharacterRowData(NewLevel)->Armor;
		SpeedAmount = GameInstance->GetCharacterRowData(NewLevel)->Speed;
		DropExpAmount = GameInstance->GetCharacterRowData(NewLevel)->DropExp;
	}


	CharacterStat.MaxHP += HpAmount;
	CharacterStat.NextExp += ExpAmount;
	if (NewLevel == 1)
	{
		CharacterStat.CurrHP = CharacterStat.MaxHP;
		//CharacterStat.CurrHP = 10;	//Test
		CharacterStat.CurrExp = 0.f;
		//if(MainHUD)
		//	MainHUD->UpdateExpBar(CharacterStat.NextExp, CharacterStat.CurrExp);
	}
	else
	{
		CharacterStat.CurrHP = CharacterStat.CurrHP + 50;
		if (CharacterStat.CurrHP > CharacterStat.MaxHP)
		{
			CharacterStat.CurrHP = CharacterStat.MaxHP;
		}
	}
	CharacterStat.Attack += AttackAmount;
	CharacterStat.Armor += ArmorAmount;
	CharacterStat.Speed += SpeedAmount;
	CharacterStat.DropExp += DropExpAmount;

	if (MainHUD)
	{
		MainHUD->UpdateHpBar(CharacterStat.MaxHP, CharacterStat.CurrHP);
		SetRandomTextureArray();
		MainHUD->SetCharacterLevel(CharacterStat.Level);
	}
	if (HealthBar != nullptr)
	{
		HealthBar->SetPercentage(CharacterStat.MaxHP, CharacterStat.CurrHP);
	}
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
	if (bDead == true)
	{
		return;
	}
	bool bFind = AbilityManager->FindAbility(Index);
	int32 FindLevel = 0;
	int32 FindIndex = 0;
	if (bFind == false)
	{
		AbilityManager->SetNewAbility();
		if (AbilityManager->GetSelectedTexture() != nullptr)
		{
			MainHUD->UpdateSkillImage(AbilityManager->GetSelectedTexture(), AbilityManager->GetSelectedIndex());
			MainHUD->UpdateSkillLevel(AbilityManager->GetSelectedIndex(), 0);
		}
	}
	else if (bFind == true)
	{
		AbilityManager->AbilityLevelUp(FindIndex, FindLevel);
		MainHUD->UpdateSkillLevel(FindIndex, FindLevel);
	}
	AbilityManager->ClearRandomArray();
	AbilityManager->ResetSelectedAbility();
	LevelUpCount--;
	MainHUD->HideChooseSkill();
	if (LevelUpCount > 0)
	{
		SetRandomTextureArray();
	}
	else
	{
		bIsLevelUp = false;
	}
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

void AGameCharacter::DeathEvent()
{
	ACharacterController* MyController = Cast<ACharacterController>(GetController());
	MyController->DisableController();

	//SetLifeSpan(5.0f);
	//SetActorHiddenInGame(true);
	//SetActorTickEnabled(false);
	Weapon->Destroy();
	bDead = true;
}

void AGameCharacter::SetGuardPoint(float Guard)
{
	MaxGuardPoint = Guard;
	CurrentGuardPoint = Guard;
	if (MainHUD)
	{
		MainHUD->UpdateGuardPoint(CurrentGuardPoint, MaxGuardPoint);
	}
	if (HealthBar != nullptr)
	{
		HealthBar->SetGuardPercent(MaxGuardPoint, CurrentGuardPoint);
	}
}

AWeapon* AGameCharacter::GetMyWeapon() const
{
	return Weapon;
}

void AGameCharacter::ExpUp(float Exp)
{
	if (bDead == true)
	{
		return;
	}
	CharacterStat.CurrExp += Exp;
	while(CharacterStat.NextExp <= CharacterStat.CurrExp)
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

bool AGameCharacter::GetIsLevelUp()
{
	return bIsLevelUp;
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
	MainHUD->SetChooseSkillImage(AbilityManager->GetRandTextureArray());
}

void AGameCharacter::SpawnHealEffect()
{
	if (HealParticles)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HealParticles, GetActorLocation(), GetActorRotation());
	}
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
	float CurrHp = CharacterStat.CurrHP;
	CharacterStat.CurrHP = hp;
	float Damage = CurrHp - CharacterStat.CurrHP;
	if (MainHUD)
	{
		MainHUD->UpdateHpBar(CharacterStat.MaxHP, CharacterStat.CurrHP);
	}
	if (HealthBar != nullptr)
	{
		HealthBar->SetPercentage(CharacterStat.MaxHP, CharacterStat.CurrHP);
	}
	CreateDamageWidget(Damage);
}

void AGameCharacter::SetDead(bool dead)
{
	bDead = dead;
}

AAbilityManager* AGameCharacter::GetAbilityManger()
{
	return AbilityManager;
}

void AGameCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	auto controller = Cast<ACharacterController>(NewController);
	controller->SetCharacter(this);

	if (IsValid(MainHUDWidgetClass))
	{
		MainHUD = Cast<UHUDWidget>(CreateWidget(GetWorld(), MainHUDWidgetClass));
		if (IsValid(MainHUD))
		{
			APlayerController* MyController = Cast<APlayerController>(GetController());
			MainHUD->SetOwningPlayer(MyController);
			if (MyController == MainHUD->GetOwningPlayer())
			{
				MainHUD->AddToViewport();
			}
		}
	}

	//bDead = false;
	//bFight = false;

	//CharacterStat.Level = 1;
	//SetNewLevel(CharacterStat.Level);


	//TEST
	if (MainHUD)
		MainHUD->UpdateExpBar(CharacterStat.NextExp, CharacterStat.CurrExp);

	if (MainHUD)
	{
		MainHUD->UpdateHpBar(CharacterStat.MaxHP, CharacterStat.CurrHP);
		SetRandomTextureArray();
		MainHUD->UpdateGuardPoint(CurrentGuardPoint,MaxGuardPoint );
	}
}

AAbilityManager* AGameCharacter::GetAbilityManager()
{
	return AbilityManager;
}
