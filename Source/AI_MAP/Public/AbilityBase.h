// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilityBase.generated.h"

UENUM(BlueprintType)
enum class AbilityType : uint8
{
	Range UMETA(DisplayName = "Range"),
	Normal UMETA(DisplayName = "Normal")
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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Attack(FVector& Location) { return; };
	virtual void Attack() { return; };
	virtual bool CheckTime();
	virtual int32 GetProjCount();
	AbilityType GetType();
protected:
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
		bool bReadyToFire;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
		int32 AbilityLevel;	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Type", meta = (AllowPrivateAccess = "true"))
		AbilityType Type;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status", meta = (AllowPrivateAccess = "true"))
		int32 ProjectileCount;

};
