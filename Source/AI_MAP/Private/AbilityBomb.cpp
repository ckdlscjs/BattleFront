// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityBomb.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"

AAbilityBomb::AAbilityBomb()
{
	CapsuleCompoent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCompoent"));
	RootComponent = CapsuleCompoent;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	MyAbilityLevel = 0;
	Damage = (MyAbilityLevel + 5) * 2;
	CoolTime = 7.f - MyAbilityLevel;
	ProjectileCount = 3;
	Type = AbilityType::Range;
	CapsuleCompoent->SetGenerateOverlapEvents(false);

}

void AAbilityBomb::BeginPlay()
{
	Super::BeginPlay();
	CapsuleCompoent->OnComponentBeginOverlap.AddDynamic(this, &AAbilityBomb::ProjectileBeginOverlap);
}

void AAbilityBomb::ProjectileBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FVector Loc = GetActorLocation();
	Loc.Z = 0.f;
	if (HitParticles)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitParticles, Loc, GetActorRotation());
	}
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, Loc, 200.f, nullptr, TArray<AActor*>(), this, false, true);
	Destroy();
}

void AAbilityBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAbilityBomb::Attack(FVector& Location)
{
	
		/*FVector Loc = Location;
		double X_Max = Loc.X + 500.f;
		double Y_Max = Loc.Y + 500.f;
		double X_Min = Loc.X - 500.f;
		double Y_Min = Loc.Y - 500.f;
		float X_Rand = FMath::RandRange(X_Min, X_Max);
		float Y_Rand = FMath::RandRange(Y_Min, Y_Max);
		Location.X = X_Rand;
		Location.Y = Y_Rand;
		Location.Z = Loc.Z;*/
		//DrawDebugSphere(GetWorld(), Location, 10, 26, FColor::Red, true, -1, 0, 2);	
}

void AAbilityBomb::SetLocation(FVector& Location)
{
	//FVector Loc = FVector::ZeroVector;
	float X_Max = Location.X + 500.f;
	float Y_Max = Location.Y + 500.f;
	float X_Min = Location.X - 500.f;
	float Y_Min = Location.Y - 500.f;
	float X_Rand = FMath::RandRange(X_Min, X_Max);
	float Y_Rand = FMath::RandRange(Y_Min, Y_Max);
	Location.X = X_Rand;
	Location.Y = Y_Rand;
	Location.Z += 1000.f;
	SetActorLocation(Location);
	CapsuleCompoent->SetGenerateOverlapEvents(true);

	return;
}
