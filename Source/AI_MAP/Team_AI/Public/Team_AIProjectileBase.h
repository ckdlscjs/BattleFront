// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI_MAP.h"
#include "GameFramework/Actor.h"
#include "Team_AIProjectileBase.generated.h"

UCLASS()
class AI_MAP_API ATeam_AIProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeam_AIProjectileBase();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ProjectileInitialize(float AttackDamage, float SpeedInit = 600.0f, float SpeedMax = 600.0f, float Gravity = 1.0f);

	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable)
	void SetCollisionEnable(bool bCollide);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Damage;
private:
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
		UStaticMeshComponent* ProjectileMesh;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
		class USphereComponent* SphereCollision;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
		class UProjectileMovementComponent* ProjectileMovementComponent;
	
};
