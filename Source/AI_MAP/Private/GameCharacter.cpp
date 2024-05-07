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
	RecognizeRange = CreateDefaultSubobject<UBoxComponent>(TEXT("Recognize Range"));
	RecognizeRange->SetupAttachment(RootComponent);

	//SpawnPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Spawn Point"));
	//SpawnPoint->SetupAttachment(SpawnBound);

	MaxNum = 5;
	CoolTimeToNormal = 15.f;
	Time = CoolTimeToNormal;
}

// Called when the game starts or when spawned
void AGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetLineOfSight();//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ô¼ï¿½ ï¿½ï¿½ï¿½ï¿½
	PrevCameraRotaion = SpringArmComp->GetComponentRotation();

	if (IsValid(MainHUDWidgetClass))
	{
		MainHUD = Cast<UHUDWidget>(CreateWidget(GetWorld(), MainHUDWidgetClass));
		if (IsValid(MainHUD))
		{
		
			MainHUD->AddToViewport();
		}
	}
	CharacterStat.Level = 1;
	SetNewLevel(CharacterStat.Level);
	AttachWeapon();
	bDead = false;
	bFight = false;
	Manager = NewObject<AAbilityManager>();
	Bomd = NewObject<AAbilityBomb>();
	Bomd->SetOwner(this);
	Manager->AbilityArray.Add(Bomd);
	
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
	FVector MyLoc = GetActorLocation();
	
	//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Actor X : %f , Y : %f , Z : %f"), MyLoc.X, MyLoc.Y, MyLoc.Z));
	FVector Loc;
	Manager->Attack(Loc);

	//CheckExp(CharacterStat.CurrExp); // ÃßÈÄ ÀÌº¥Æ® Çü½ÄÀ¸·Î ¹Ù²Ù°í ½Í´Ù...
}

void AGameCharacter::TakenDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigateBy, AActor* DamageCauser)
{
	CharacterStat.CurrHP -= Damage;	
	if (CharacterStat.CurrHP <= 0.f)
	{
		//Player Die Event;
		bDead = true;
	}
	MainHUD->UpdateHpBar(CharacterStat.MaxHP, CharacterStat.CurrHP);
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
	//float test1 = GameInstance->GetCharacterRowData(NewLevel)->Ability;
	//float test2 = GameInstance->GetCharacterRowData(NewLevel)->Percentage;
	
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
	bool bFind = FindAbility(Index);
	if (bFind == false)
	{
		if (SkillArray.Num() >= 5)
		{
			return;
		}
		AbilityMap.Add(SelectedAbility, SkillIndex);
		FSkillData SkillData;
		SkillData.Index = SkillIndex;
		SkillData.Level = 0;
		SkillArray.Add(SkillData.Index, SkillData.Level);
		MainHUD->UpdateSkillImage(SelectedAbility, SkillData.Index);
		MainHUD->UpdateSkillLevel(SkillData.Index, SkillData.Level);
		SkillIndex++;
		
	}
	else if (bFind == true)
	{
		int32 FindIndex = AbilityMap.FindRef(SelectedAbility);
		int32& FindLevel = SkillArray[FindIndex];
		FindLevel += 1;
		MainHUD->UpdateSkillLevel(FindIndex, FindLevel);
	}
	RandomTexture.Empty();
	MainHUD->HideChooseSkill();
}

void AGameCharacter::AttachWeapon()
{
	Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass,FVector::ZeroVector, FRotator::ZeroRotator);
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

void AGameCharacter::SetRandomTextureArray()
{
	MainHUD->ShowChooseSkill();
	if (SkillArray.Num() < 5)
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
	}
	MainHUD->SetChooseSkillImage(RandomTexture);
}