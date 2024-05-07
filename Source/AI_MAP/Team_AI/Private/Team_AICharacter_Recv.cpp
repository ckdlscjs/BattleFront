// Fill out your copyright notice in the Description page of Project Settings.


#include "Team_AICharacter_Recv.h"

void ATeam_AICharacter_Recv::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AIAttack.BindLambda([this]() -> void
		{
			BehaviorAttack();
		});
	AIDead.BindLambda([this]() -> void
		{
			BehaviorDead();
		});
	AIMove.BindLambda([this](FVector location, FRotator rotate, float movespeed) -> void
		{
			SetActorLocation(location);
			SetActorRotation(rotate);
			SpeedCurrent = movespeed;
		});
}

void ATeam_AICharacter_Recv::RecvTest(FString str)
{
	//connect networkmanager
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("%s"), *str), true, true, FColor::Red);
	if (str == "MoveTo")
		AIMove.Execute(GetActorLocation(), GetActorRotation(), 100);
	else if (str == "Attack")
		AIAttack.Execute();
	else if (str == "Dead")
		AIDead.Execute();
	else
		SpeedCurrent = 0;
}
