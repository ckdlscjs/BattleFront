// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilityBase.generated.h"

UENUM(BlueprintType)
enum class AbilityType : uint8
{
	Range UMETA(DisplayName = "Range"),
	Normal UMETA(DisplayName = "Normal"),
	Drone UMETA(DisplayName = "Drone"),
	Heal UMETA(DisplayName = "Heal"),
	Guard UMETA(DisplayName = "Guard")
};
UCLASS()
class AI_MAP_API AAbilityBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbilityBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void ProjectileBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
	{ return;};

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Attack(FVector& Location) { return; };// 필 구현
	virtual void Attack() { return; };// 필 구현
	virtual bool CheckTime(float DTimer);
	virtual int32 GetProjCount();
	AbilityType GetType();
	virtual class UTexture2D* GetTexture();
	virtual void SetAbilityLevel(int32 Level);
	virtual int32 GetAbilityLevel();
	virtual FString GetAbilityName();
	virtual void AbilityLevelUp();
	virtual void SetLocation(FVector& Location) { return ; };
	virtual void HealCharacterHp(float& HealthPoint, float MaxPoint) { return; };
	virtual void ReturnToTarget(FVector& Location) { return; };
	virtual void ChargeGuardPoint() { return; };
	virtual float GetAbilityDetail() { return 0.f; };
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* CapsuleCompoent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components",meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status", meta = (AllowPrivateAccess = "true"))
		float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status", meta = (AllowPrivateAccess = "true"))
		float CoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status", meta = (AllowPrivateAccess = "true"))
		float FireTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status", meta = (AllowPrivateAccess = "true"))
		int32 ProjectileCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status", meta = (AllowPrivateAccess = "true"))
		bool bReadyToFire;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Status", meta = (AllowPrivateAccess = "true"))
		FString Name;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status", meta = (AllowPrivateAccess = "true"))
		int32 MyAbilityLevel;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Type", meta = (AllowPrivateAccess = "true"))
		AbilityType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture", meta = (AllowPrivateAccess = "true"))
		class UTexture2D* AbilityTexture;

};
