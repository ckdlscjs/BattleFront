// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

	UPROPERTY()
		float CurrHP;
	UPROPERTY()
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
	const void GetSpringArmRotator(FRotator& Rotator);
	const void GetSpringArmLocation(FVector& Location);
	const int32 GetCharacterLevel();
	const bool IsDead();

	UWorld* GetMyWorld();
	void SetSpringArmLocation(const FVector& Location);
	void AddSpringArmRotaion(const FRotator& Rotator);
	void SaveSpringArmRotation();
	void SetRandomTextureArray();
	void DetachSpringArm();
	void AttachSpringArm();
	void ResetSpringArmRotation();
	void SetNewLevel(int32 NewLevel);	
	void SetAbility(int32 Index);
	void AttachWeapon();
	void SetFightState(bool IsFight);
	void SetAimYaw(float Yaw);
	void SetCharacterMovementRotation(bool bState);
	void Fire();

	void ExpUp(float Exp);//Test

	bool FindAbility(int32 Index);
	const float GetDamage();
	bool IsFight();
	float GetAimYaw();

	UPROPERTY(EditAnywhere)
	float CharacterSightRange = 700.f;
	UFUNCTION(BlueprintImplementableEvent)
		void SetLineOfSight();

private:
	UFUNCTION()
		void TakenDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigateBy, AActor* DamageCauser);
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* RecognizeRange;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = true))
		class AWeapon* Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = true))
		FName WeaponSocket= TEXT("WeaponSocket");
	UPROPERTY(EditAnywhere,  Category = "Weapon", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AWeapon> WeaponClass;

	FRotator PrevCameraRotaion;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
		FCharacterStat CharacterStat;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UUserWidget> MainHUDWidgetClass;
	UPROPERTY()
		class UHUDWidget* MainHUD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture", meta = (AllowPrivateAccess = "true"))
		TArray<class UTexture2D*> TextureArray; // GameMode�� �̵���Ű�°� ���ƺ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 MaxNum;
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
		bool bFight;
	UPROPERTY()
		float AimYaw;
	UPROPERTY()
		class AAbilityManager* Manager; // for Test
	UPROPERTY()
		class AAbilityBomb* Bomd;

	
};
