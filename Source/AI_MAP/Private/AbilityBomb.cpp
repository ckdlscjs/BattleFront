// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityBomb.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"

AAbilityBomb::AAbilityBomb()
{
	Damage = AbilityLevel * 2;
	CoolTime = 7.f - AbilityLevel;
	ProjectileCount = 3;
	Type = AbilityType::Range;
}

void AAbilityBomb::BeginPlay()
{
	Super::BeginPlay();
}

void AAbilityBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAbilityBomb::Attack(FVector& Location)
{
	if (Super::CheckTime())
	{
		for (int i = 0; i < ProjectileCount; i++)
		{
			UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Shoooooooot!!!~!")));
			auto MyOwner = GetOwner();
			FVector Loc = MyOwner->GetActorLocation();
			double X_Max = Loc.X + 500.f;
			double Y_Max = Loc.Y + 500.f;
			double X_Min = Loc.X - 500.f;
			double Y_Min = Loc.Y - 500.f;
			float X_Rand = FMath::RandRange(X_Min, X_Max);
			float Y_Rand = FMath::RandRange(Y_Min, Y_Max);
			Location.X = X_Rand;
			Location.Y = Y_Rand;
			Location.Z = Loc.Z;
			DrawDebugSphere(MyOwner->GetWorld(), Location, 10, 26, FColor::Red, true, -1, 0, 2);
		}
	}
}
