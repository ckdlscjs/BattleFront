// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"



UCLASS()
class AI_MAP_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	virtual void SetDamage(float Damage);
	virtual void Shot();

	UFUNCTION(BlueprintCallable)
	void SetVisibility(bool visible);

	virtual void SetDepthStencil();
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
		UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
		USceneComponent* EffectSpawnPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UParticleSystemComponent* BulletParticleSystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Value", meta = (AllowPrivateAccess = true))
		float WeaponDamage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Socket", meta = (AllowPrivateAccess = true))
		USceneComponent* ProjectileSpawnPoint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet", meta = (AllowPrivateAccess = true))
		TSubclassOf<class AProjectile> ProjectileClass;

public:
	class UNetworkManager* GetNetworkManager() const;
	TSubclassOf<class AProjectile> GetPrjClass();
};
