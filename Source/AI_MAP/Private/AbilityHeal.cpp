// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityHeal.h"
#include "Kismet/GameplayStatics.h"

AAbilityHeal::AAbilityHeal()
{
	MyAbilityLevel = 0;
	HealAmount = 5.f;
	CoolTime = 7.f;
	Type = AbilityType::Heal;
}

void AAbilityHeal::BeginPlay()
{
	Super::BeginPlay();
}

void AAbilityHeal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAbilityHeal::HealCharacterHp(float& HealthPoint, float MaxPoint)
{
	HealthPoint += HealAmount;
	if (HealthPoint >= MaxPoint)
	{
		HealthPoint = MaxPoint;
	}
	if (HealSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HealSound, GetActorLocation());
	}
}

void AAbilityHeal::AbilityLevelUp()
{
	Super::AbilityLevelUp();
	HealAmount++;
	CoolTime--;
}

float AAbilityHeal::GetAbilityDetail()
{
	return HealAmount;
}

void AAbilityHeal::SetAbilityDetail(float Details)
{
	HealAmount = Details;
}
