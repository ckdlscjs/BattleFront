// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityProtected.h"


AAbilityProtected::AAbilityProtected()
{

	MyAbilityLevel = 0;
	AbilityPoint = 10.f;
	GuardPoint = ((MyAbilityLevel + 1) * 5) + AbilityPoint;
	CoolTime = 30.f;
	Type = AbilityType::Guard;
}

void AAbilityProtected::BeginPlay()
{
	Super::BeginPlay();

}

void AAbilityProtected::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


float AAbilityProtected::GetAbilityDetail()
{
	return GuardPoint;
}

void AAbilityProtected::ChargeGuardPoint()
{

	GuardPoint = ((MyAbilityLevel + 1) * 5) + AbilityPoint;

}

void AAbilityProtected::AbilityLevelUp()
{
	Super::AbilityLevelUp();
	AbilityPoint++;;
	GuardPoint = ((MyAbilityLevel + 1) * 5) + AbilityPoint;
	CoolTime -= 3;
}

void AAbilityProtected::SetAbilityDetail(float Details)
{
	GuardPoint = Details;
}
