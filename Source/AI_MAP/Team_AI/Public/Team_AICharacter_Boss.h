// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Team_AICharacterBase.h"
#include "Team_AICharacter_Boss.generated.h"

/**
 * 
 */
class USphereComponent;
class UBoxComponent;
UCLASS()
class AI_MAP_API ATeam_AICharacter_Boss : public ATeam_AICharacterBase
{
	GENERATED_BODY()
public:

	ATeam_AICharacter_Boss();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called PostInitialize
	virtual void PostInitializeComponents() override;
	// Called to bind functionality to input
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	bool RecognizePlayers();
	UFUNCTION(BlueprintCallable)
	int32 GetRandomAttackIdx() const;
	
protected:
private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void MeleeAttackCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* RecognizeRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> Players;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRandomStream randVar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* MeleeAttackCollision;

	float AmountKnockback;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amount_Knockback1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amount_Knockback3;

	UPROPERTY(EditAnywhere)
	FTimerHandle BossAttackTimerHandle;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Attack4Delay;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Attack4Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* BossModifyCollision;
private:
};
