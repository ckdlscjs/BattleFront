// Fill out your copyright notice in the Description page of Project Settings.


#include "Team_AIMagneticField.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATeam_AIMagneticField::ATeam_AIMagneticField()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponet"));
	MagneticField = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MagneticField"));
	MagneticField->SetupAttachment(RootComponent);
	SafetySphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SafetySphere"));
	SafetySphere->SetupAttachment(RootComponent);
	CurrentPhase = 1;
	CurrentState = ESafetyState::PAUSE;
	MagenticPhase.Add({ EMagneticPhase::PHASEDEFULAT, EPhaseTime::PHASEDEFULAT });
	MagenticPhase.Add({ EMagneticPhase::PHASE0, EPhaseTime::PHASE0 });
	MagenticPhase.Add({ EMagneticPhase::PHASE1, EPhaseTime::PHASE1 });
	MagenticPhase.Add({ EMagneticPhase::PHASE2, EPhaseTime::PHASE2 });
	MagenticPhase.Add({ EMagneticPhase::PHASE3, EPhaseTime::PHASE3 });
	MagenticPhase.Add({ EMagneticPhase::PHASE4, EPhaseTime::PHASE4 });
}

// Called when the game starts or when spawned
void ATeam_AIMagneticField::BeginPlay()
{
	Super::BeginPlay();
	MagneticField->OnComponentBeginOverlap.AddDynamic(this, &ATeam_AIMagneticField::OnMagneticFieldBeginOverlap);
	MagneticField->OnComponentEndOverlap.AddDynamic(this, &ATeam_AIMagneticField::OnMagneticFieldEndOverlap);
	SafetySphere->OnComponentBeginOverlap.AddDynamic(this, &ATeam_AIMagneticField::OnSafetySphereBeginOverlap);
	SafetySphere->OnComponentEndOverlap.AddDynamic(this, &ATeam_AIMagneticField::OnSafetySphereEndOverlap);
}

void ATeam_AIMagneticField::RandomPoint()
{
	if (CurrentPhase >= MagenticPhase.Num() - 1)
		return;
	FVector Extent = SafetySphere->GetStaticMesh()->GetBoundingBox().GetExtent();
	double RadiusCurrent = Extent.X * (float)MagenticPhase[CurrentPhase].Key * 0.01f;
	double RadiusNext = Extent.X * (float)MagenticPhase[CurrentPhase + 1].Key * 0.01f;
	double RandRadius = UKismetMathLibrary::Sqrt(UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f)) * (RadiusCurrent - RadiusNext);
	double RandAngle = UKismetMathLibrary::RandomFloatInRange(0.0f, UKismetMathLibrary::GetPI() * 2.0f);
	double RandPosX = UKismetMathLibrary::Cos(RandAngle) * RandRadius;
	double RandPosY = UKismetMathLibrary::Sin(RandAngle) * RandRadius;
	FVector rand(RandPosX, RandPosY, 0.0f);
	SafetySphere->AddLocalOffset(rand);
}

void ATeam_AIMagneticField::ShrinkSafetyField(float DeltaTime)
{
	/*
		enum class ESafetyState : uint8
	{
		SHRINK UMETA(DisplayName = "Shrink"),
		PAUSE UMETA(DisplayName = "Pause"),
		END UMETA(DisplayName = "End"),
	};

		enum class EMagneticPhase : uint8
	{
		PHASEDEFULAT = 0,
		PHASE0 = 100 UMETA(DisplayName = "Phase0"),
		PHASE1 = 80 UMETA(DisplayName = "Phase1"),
		PHASE2 = 60 UMETA(DisplayName = "Phase2"),
		PHASE3 = 30 UMETA(DisplayName = "Phase3"),
		PHASE4 = 5 UMETA(DisplayName = "Phase4"),
	};

		enum class EPhaseTime : uint8
	{
		PHASEDEFULAT = 0,
		PHASE0 = 60 UMETA(DisplayName = "Time0"),
		PHASE1 = 50 UMETA(DisplayName = "Time1"),
		PHASE2 = 30 UMETA(DisplayName = "Time2"),
		PHASE3 = 20 UMETA(DisplayName = "Time3"),
		PHASE4 = 10 UMETA(DisplayName = "Time4"),
	};*/
	CurrentTime += DeltaTime;
	float PrevRatio = (float)MagenticPhase[CurrentPhase-1].Key / 100.0f;
	float CurrentRatio = (float)MagenticPhase[CurrentPhase].Key / 100.0f;
	float ScaleRatio = CurrentTime / (float)MagenticPhase[CurrentPhase].Value;
	float CurrentScale = PrevRatio + (CurrentRatio - PrevRatio) * ScaleRatio;
	SafetySphere->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f)* CurrentScale);
	if (CurrentTime >= (float)MagenticPhase[CurrentPhase].Value)
	{
		RandomPoint();
		CurrentState = ESafetyState::PAUSE;
		CurrentTime = 0.0f;
		CurrentPhase++;
		if (CurrentPhase >= MagenticPhase.Num())
			CurrentState = ESafetyState::END;
	}
}

void ATeam_AIMagneticField::OnMagneticFieldBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OverlapActors.Add(OtherActor);
}

void ATeam_AIMagneticField::OnMagneticFieldEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlapActors.Remove(OtherActor);
}

void ATeam_AIMagneticField::OnSafetySphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OverlapActors.Remove(OtherActor);
}

void ATeam_AIMagneticField::OnSafetySphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlapActors.Add(OtherActor);
}

// Called every frame
void ATeam_AIMagneticField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (AActor* actor : OverlapActors)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("%s"), *actor->GetName()));
	}
	if (CurrentState == ESafetyState::SHRINK)
		ShrinkSafetyField(DeltaTime);
}

