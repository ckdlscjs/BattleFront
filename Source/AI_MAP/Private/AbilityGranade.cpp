// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityGranade.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h" 
#include "Kismet/KismetMathLibrary.h"
#include "GrenadeFireArea.h"
#include "AI_MAP.h"
AAbilityGranade::AAbilityGranade()
{

	CapsuleCompoent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCompoent"));
	RootComponent = CapsuleCompoent;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	StaticMesh->SetupAttachment(RootComponent);
	MyAbilityLevel = 0;
	Damage = (MyAbilityLevel + 6) * 2;
	CoolTime = 7.f - MyAbilityLevel;
	ProjectileCount = 1;
	Type = AbilityType::Range;
	CapsuleCompoent->SetGenerateOverlapEvents(false);

	//ProjectileMovement->InitialSpeed = 1000.f;
//	ProjectileMovement->MaxSpeed = 1500.f;
}

void AAbilityGranade::BeginPlay()
{
	Super::BeginPlay();
	CapsuleCompoent->OnComponentBeginOverlap.AddDynamic(this, &AAbilityGranade::ProjectileBeginOverlap);

	int a = 0;
	a = 10;

}

void AAbilityGranade::ProjectileBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CapsuleCompoent->SetGenerateOverlapEvents(false);
	UKismetSystemLibrary::PrintString(GetWorld(), FString(TEXT("Overlap!")));
	FVector Loc = SweepResult.Location;
	Loc.Z = 0;
	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, Loc, GetActorRotation());
	}
	
	AGrenadeFireArea* FireArea = GetWorld()->SpawnActor<AGrenadeFireArea>(FireAreaClass, Loc,FRotator::ZeroRotator);
	auto MyOwner = GetOwner();
	FireArea->SetOwner(MyOwner);
	Destroy();
	// spawn FireArea

	// Destroy;
}

void AAbilityGranade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AAbilityGranade::SetLocation(FVector& Location)
{
	FVector StartLocation = Location;
	FVector TargetLocation = Location;
	StartLocation.X -= 1000.f;
	float Y_Min = Location.Y - 1000.f;
	float Y_Max = Location.Y + 1000.f;
	float Y_Rand = FMath::RandRange(Y_Min, Y_Max);
	StartLocation.Y = Y_Rand;
	StartLocation.Z += 1500.f;
	FVector RandVec = FMath::VRand();
	RandVec.Z = 0;
	//SetActorLocation(Location);
	CapsuleCompoent->SetGenerateOverlapEvents(true);

	ProjectileMovement->AddForce(RandVec * 100000);


	//float Force = FMath::RandRange(200, 500);
	//ProjectileMovement->AddForce(Temp * Force);
	/*FVector StartLocation = Location;
	SetActorLocation(StartLocation);*/

	/*float X_Max = Location.X + 300.f;
	float Y_Max = Location.Y + 300.f;
	float X_Min = Location.X - 300.f;
	float Y_Min = Location.Y - 300.f;
	float X_Rand = FMath::RandRange(X_Min, X_Max);
	float Y_Rand = FMath::RandRange(Y_Min, Y_Max);
	Location.X = X_Rand;
	Location.Y = Y_Rand;
	Location.Z = 0.f;
	FVector TargetLocation = Location;
	FVector Temp = TargetLocation - StartLocation;*/

//	ProjectileMovement->AddForce(Temp * 300);
	
	return;
}
