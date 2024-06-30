// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Protocol.pb.h"
#include "GameCharacter.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStat
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 Level;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float MaxHP;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Attack;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Armor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Speed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float DropExp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float NextExp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float CurrHP;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float CurrExp;
};


USTRUCT(BlueprintType)
struct FSkillData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 Index;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 Level;
};
UCLASS()
class AI_MAP_API AGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGameCharacter();
	virtual ~AGameCharacter();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInsigator, AActor* DamageCauser) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginDestroy() override;
	const void GetSpringArmRotator(FRotator& Rotator);
	const void GetSpringArmLocation(FVector& Location);
	const int32 GetCharacterLevel();
	const bool IsDead();

	UWorld* GetMyWorld();
	void CreateDamageWidget(float DamageAmount);
	void SetSpringArmLocation(const FVector& Location);
	void AddSpringArmRotaion(const FRotator& Rotator);
	void SaveSpringArmRotation();
	void SetRandomTextureArray();
	void SpawnHealEffect();
	void DetachSpringArm();
	void AttachSpringArm();
	void ResetSpringArmRotation();
	void SetNewLevel(int32 NewLevel);	
	void SetAbility(int32 Index);
	void AttachWeapon();
	void SetFightState(bool IsFight);
	void SetAimYaw(float Yaw);
	void PlayShotSound();
	void SetCharacterMovementRotation(bool bState);
	void Fire();
	void UpdateHpWiget();
	void DeathEvent();
	void SetGuardPoint(float Guard);
	void UpdateGuradUI(float Max, float Cur = 0.f);
	class AWeapon* GetMyWeapon() const;
	void ExpUp(float Exp);//Test

	bool FindAbility(int32 Index);
	bool IsFight();
	bool GetIsLevelUp();
	const float GetDamage();
	
	float GetAimYaw();
	float& GetCurrentHP();
	float GetCurrentMaxHp();
	float GetCurrentGuard();
	
	UPROPERTY(EditAnywhere)
	float CharacterSightRange = 700.f;

	UFUNCTION(BlueprintCallable)
	void SetVisibility(bool visible);
	UFUNCTION(BlueprintCallable)
		void CharacterMaxHpUp(float value);
	UFUNCTION(BlueprintCallable)
		void CharacterLevelUp();
	UFUNCTION(BlueprintCallable)
		void CharacterAttackUp(float value);
	UFUNCTION(BlueprintCallable)
		void CharacterEatItem(float value);

	UFUNCTION(BlueprintCallable)
	void SetPlayerColor();
	UFUNCTION(BlueprintCallable)
	class UHUDWidget* GetHUDWidget();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperSpriteComponent* PaperSprite;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperSprite* SpritePlayer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UParticleSystemComponent* LevelUpParticleSystemComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UNiagaraSystem* HealParticles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FColor> PlayerColors;
private:
	UFUNCTION()
		void TakenDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigateBy, AActor* DamageCauser);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* RecognizeVisibility;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere , Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USceneCaptureComponent2D* SceneCaptureComponent;
	UPROPERTY(EditAnywhere, Category = "Capture")
		class UTextureRenderTarget2D* RenderTarget;
	UPROPERTY(VisibleAnywhere)
		bool bIsCapture;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DamageActor", meta = (AllowPrivateAccess = "true"))
		class ADamageTextActor* DamageTextActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DamageActor", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class ADamageTextActor> DamageTextClass;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Socket", meta = (AllowPrivateAccess = true))
		USceneComponent* AbilitySpawnPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Socket", meta = (AllowPrivateAccess = true))
		USceneComponent* DamageTextPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = true))
		class AWeapon* Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Manager", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AAbilityManager> AbilityManagerClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class AAbilityManager* AbilityManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = true))
		FName WeaponSocket= TEXT("WeaponSocket");
	UPROPERTY(EditAnywhere,  Category = "Weapon", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AWeapon> WeaponClass;
	
	FRotator PrevCameraRotaion;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
		FCharacterStat CharacterStat;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
		float MaxGuardPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
		float CurrentGuardPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UUserWidget> MainHUDWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY()
		class UHUDWidget* MainHUD;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
		class UHealthBar* HealthBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture", meta = (AllowPrivateAccess = "true"))
		TArray<class UTexture2D*> TextureArray; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 MaxNum;
	UPROPERTY(EditAnywhere, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		class USoundBase* ShootSound;
	UPROPERTY(EditAnywhere, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		class USoundBase* LevelUpSound;
	UPROPERTY(EditAnywhere, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		class USoundBase* ItemEquipSound;
	UPROPERTY(EditAnywhere, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		class USoundBase* HitSound;
	UPROPERTY()
		class UTexture2D* SelectedAbility;

	UPROPERTY()
		TArray<class UTexture2D*> RandomTexture;
	UPROPERTY()
		TMap<class UTexture2D*, int32> AbilityMap;
	UPROPERTY()
		TMap<int32 , int32> SkillArray;
	UPROPERTY()
		TMap<int32, class UTexture2D*> Ability_Index;
	UPROPERTY()
		int32 SkillIndex = 0;
	UPROPERTY()
		bool bDead;
	UPROPERTY()
		float CoolTimeToNormal;
	UPROPERTY()
		float Time;
	UPROPERTY()
		int32 LevelUpCount;
	UPROPERTY()
		bool bFight;
	UPROPERTY()
		float AimYaw;
	UPROPERTY()
		class AAbilityBomb* Bomd;
	UPROPERTY()
		bool bIsLevelUp;
	UPROPERTY()
		bool bChangeColor;
	UPROPERTY()
		class ATeam_AIGameMode* MyGameMode;
	UPROPERTY()
		class UNetworkManager* NetworkManager;
public:
	bool IsMyPlayer();

	Protocol::MoveState GetMoveState() { return PlayerInfo->state(); }
	void SetMoveState(Protocol::MoveState State);

public:
	void SetPlayerInfo(const Protocol::PosInfo& Info);
	void SetDestInfo(const Protocol::PosInfo& Info);
	Protocol::PosInfo* GetPlayerInfo() { return PlayerInfo; }

public:
	class Protocol::PosInfo* PlayerInfo;
	class Protocol::PosInfo* DestInfo;

	FVector2D DesiredInput;
	FVector DesLoc;
	FRotator DesRot;
	FVector DesVel;
	float   speed;

	bool bReceived = false;

	AWeapon* GetWeapon();
	class UNetworkManager* GetNetworkManager() const;
	class ATeam_AIGameMode* GetTeam_AIGameMode();
	void UpdateHP(float hp);
	void UpdateGuard(float guard);
	void SetDead(bool dead);
	virtual void PossessedBy(AController* NewController) override;
	class AAbilityManager* GetAbilityManager();

	void RecvCharacterMaxHpUp(float value);
	void RecvCharacterLevelUp(int64 lv);
	void RecvCharacterAttackUp(float value);
	void RecvCharacterEatItem(float value);

	void RecvExpUp(float exp);
	void RecvLvUP(int64 level, float currHP);
};
