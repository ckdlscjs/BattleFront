// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "AbilityGranade.generated.h"

/**
 *
 */
UCLASS()
class AI_MAP_API AAbilityGranade : public AAbilityBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void ProjectileBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
public:
	AAbilityGranade();
	virtual void Tick(float DeltaTime) override;
	virtual void AbilityLevelUp() override;
	virtual int32 GetProjCount() override;
	virtual float GetAbilityDetail() override;
	virtual void SetAbilityDetail(float Details) override;
	virtual void SetLocation(FVector& Location, int abilityIdx) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireArea", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AGrenadeFireArea> FireAreaClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Component" , meta = (AllowPrivateAccess = "true"))
		class UParticleSystem* HitParticles;
	UPROPERTY(EditAnywhere, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		class USoundBase* GrenadeSound;
public:
	void SetForce(FVector force);
};
