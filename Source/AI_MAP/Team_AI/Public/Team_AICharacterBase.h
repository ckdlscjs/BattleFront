// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "AI_MAP.h"
#include "GameFramework/Character.h"
#include "Protocol.pb.h"
#include "Team_AICharacterBase.generated.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	IDLE UMETA(DisplayName = "Idle"),
	PATROL UMETA(DisplayName = "Patrol"),
	INVESTIGATING UMETA(DisplayName = "Investigating"),
	ATTACK UMETA(DisplayName = "Attack"),
	DEAD UMETA(DisplayName = "Dead"),
};

UCLASS()
class AI_MAP_API ATeam_AICharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATeam_AICharacterBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called PostInitialize
	virtual void PostInitializeComponents() override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	float GetCurrentSpeed();

	//hyuk add
	UFUNCTION(BlueprintCallable)
	void SetCurrentSpeed(float speed);

	UFUNCTION(BlueprintCallable)
	bool IsDead() const;

	UFUNCTION(BlueprintCallable)
	bool IsAttacking() const;

	FName GetTargetTag() const;
	float GetAttackRange() const;
	float GetAttackDelay() const;
	float GetDetectRadius();
	float GetPatrolRadius();
	UFUNCTION(BlueprintCallable, CallInEditor)
	virtual void BehaviorAttack(int idx = 0);
	UFUNCTION(BlueprintCallable, CallInEditor)
	void BehaviorDead();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void BehviorSpawn();
	UFUNCTION()
	void OnDeadMontageEnd(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION(BlueprintCallable)
	ATeam_AIPatrolRoute* GetPatrolRoute() const;
	UFUNCTION(BlueprintCallable)
	void SetPatrolRoute(ATeam_AIPatrolRoute* Route);
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentPatrolIndex() const;
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentPatrolDirection() const;
	UFUNCTION(BlueprintCallable)
	void IncrementPatrolIndex();
	UFUNCTION(BlueprintCallable)
	void SetCurrentState(ECharacterState State);
	UFUNCTION(BlueprintCallable)
	ECharacterState GetCurrentState();
	UFUNCTION(BlueprintCallable)
	virtual void SendTest(FString str, int idx = 0);
	UFUNCTION(BlueprintCallable)
	virtual void RecvTest(FString str);

	UFUNCTION(BlueprintCallable)
	virtual void ActivateParticleSystem(FString str);

	UFUNCTION(BlueprintCallable)
	virtual const class UParticleSystemComponent* GetParticleSystemComponent(FString str) const;

	UFUNCTION(BlueprintCallable)
	virtual void AttackParticletoActors(FString str = "");
protected:

private:
	UFUNCTION(BlueprintCallable)

	void TakenDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	void SetStat(int32 CharacterLevel);

public:
	void CreateDamageWidget(float DamageAmount);
	void SetHealthBarPercent(float Max, float Cur);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DamageActor", meta = (AllowPrivateAccess = "true"))
		class ADamageTextActor* DamageTextActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DamageActor", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class ADamageTextActor> DamageTextClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Socket", meta = (AllowPrivateAccess = true))
		USceneComponent* DamageTextPoint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UUserWidget> HealthBarWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
		class UHealthBar* HealthBar;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent* HealthBarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName TargetTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AttackDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AnimationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DetectRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PatrolRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ProjectileSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float EnemyStatDecreaseAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Armor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DropExp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float SpeedCurrent;

	UPROPERTY()
		class UTeam_AIAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ATeam_AIPatrolRoute* PatrolRoute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PatrolIndex;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PatrolDirection;

	/*Range
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* ProjectileSpawnPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ATeam_AIProjectileBase> ProjectileClass;
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECharacterState CurrentState;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UParticleSystemComponent* AttackParticleSystem;*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, class UParticleSystemComponent*> ParticleSystems;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ATeam_AICharacterBase* RecvAI;*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInstance* HitOverlayMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HitOverlayTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> Players;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle DamageOverlayHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperSpriteComponent* PaperSprite;
private:


// Server
public:
	Protocol::PosInfo pos;
	class UNetworkManager* GetNetworkManager() const;
	void SetCurHP(float hp);
};
