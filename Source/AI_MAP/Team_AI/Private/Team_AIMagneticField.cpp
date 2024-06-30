// Fill out your copyright notice in the Description page of Project Settings.


#include "Team_AIMagneticField.h"
#include "Team_AIGameMode.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "NetworkManager.h"
#include "Protocol.pb.h"
#include "Team_AIGameMode.h"
#include "GameCharacter.h"

// Sets default values
ATeam_AIMagneticField::ATeam_AIMagneticField()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponet"));
	MagneticField = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MagneticField"));
	MagneticField->SetupAttachment(RootComponent);
	SafetySphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SafetySphere"));
	SafetySphere->SetupAttachment(RootComponent);
	CurrentPhase = 0;
	//CurrentState = ESafetyState::PAUSE;
	MagenticPhase.Add({ EMagneticPhase::PHASEDEFULAT, EPhaseTime::PHASEDEFULAT });
	MagenticPhase.Add({ EMagneticPhase::PHASE0, EPhaseTime::PHASE0 });
	MagenticPhase.Add({ EMagneticPhase::PHASE1, EPhaseTime::PHASE1 });
	MagenticPhase.Add({ EMagneticPhase::PHASE2, EPhaseTime::PHASE2 });
	MagenticPhase.Add({ EMagneticPhase::PHASE3, EPhaseTime::PHASE3 });
	MagenticPhase.Add({ EMagneticPhase::PHASE4, EPhaseTime::PHASE4 });
	DamageDelay = 1.0f;
	CenterCur = SafetySphere->GetRelativeLocation();
	CenterNext = CenterCur;
	PauseTime = 5.0f;
	Damage = 0;
}

// Called when the game starts or when spawned
void ATeam_AIMagneticField::BeginPlay()
{
	Super::BeginPlay();
	UpdateSafetyFieldValue();
}

void ATeam_AIMagneticField::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}

void ATeam_AIMagneticField::BeginDestroy()
{
	Super::BeginDestroy();
	if (GetWorld())
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

float ATeam_AIMagneticField::GetRemainTime() const
{
	return retTime;
}

void ATeam_AIMagneticField::RandomPoint()
{
	if (CurrentPhase >= MagenticPhase.Num() - 1)
		return;
	FVector Extent = SafetySphere->GetStaticMesh()->GetBoundingBox().GetExtent();
	double RadiusCurrent = Extent.X * (float)MagenticPhase[CurrentPhase].Key * 0.01f * GetActorScale3D().X;
	double RadiusNext = Extent.X * (float)MagenticPhase[CurrentPhase + 1].Key * 0.01f * GetActorScale3D().X;
	double RandDiff = UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f) * (RadiusCurrent - RadiusNext);
	double RandAngle = UKismetMathLibrary::RandomFloatInRange(0.0f, UKismetMathLibrary::GetPI() * 2.0f);
	double RandPosX = UKismetMathLibrary::Cos(RandAngle) * RandDiff;
	double RandPosY = UKismetMathLibrary::Sin(RandAngle) * RandDiff;
	FVector rand(RandPosX, RandPosY, 0.0f);
	CenterCur = CenterNext;
	CenterNext = rand;
}

void ATeam_AIMagneticField::ShrinkSafetyField()
{
	if (CurrentPhase >= MagenticPhase.Num() - 1)
		return;
	//CurrentState = ESafetyState::SHRINK;
	RandomPoint();
	float scaleCurrent = (float)MagenticPhase[CurrentPhase].Key;
	float scaleNext = (float)MagenticPhase[CurrentPhase + 1].Key;
	FVector Dir = CenterNext - CenterCur;
	Dir.Normalize();

	GetWorld()->GetTimerManager().ClearTimer(MagenticTimer);
	GetWorld()->GetTimerManager().ClearTimer(PauseTimer);
	GetWorld()->GetTimerManager().SetTimer
	(
		MagenticTimer,
		[=]() -> void
		{
			if (!IsValid(this))
				return;
			if (!GetWorld()->GetTimerManager().IsTimerActive(MagenticTimer))
				return;
			CurrentTime += GetWorld()->GetDeltaSeconds();
			retTime = retTime = std::max((float)MagenticPhase[CurrentPhase].Value - CurrentTime, 0.0f);
			float ratio = CurrentTime / (float)MagenticPhase[CurrentPhase].Value;
			float scale = (scaleNext + (scaleCurrent - scaleNext) * (1.0f - ratio)) * 0.01f;
			SafetySphere->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f) * scale);
			SafetySphere->AddLocalOffset(Dir * GetWorld()->GetDeltaSeconds());
			UpdateSafetyFieldValue();
			if (CurrentTime > (float)MagenticPhase[CurrentPhase].Value)
			{
				CurrentTime = 0.0f;
				GetWorld()->GetTimerManager().PauseTimer(MagenticTimer);
				CurrentPhase = std::min(CurrentPhase + 1, (int32)MagenticPhase.Num() - 1);
				//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("CurrentPhase : %d"), CurrentPhase));
				PauseSafetyField();
			}
		},
		GetWorld()->GetDeltaSeconds(),
		true,
		0.0f
	);
}

void ATeam_AIMagneticField::PauseSafetyField()
{
	if (CurrentPhase >= MagenticPhase.Num() - 1)
		return;
	Damage += 1;
	//CurrentState = ESafetyState::PAUSE;
	GetWorld()->GetTimerManager().ClearTimer(MagenticTimer);
	GetWorld()->GetTimerManager().ClearTimer(PauseTimer);
	GetWorld()->GetTimerManager().SetTimer
	(
		PauseTimer,
		[this]() -> void
		{
			if (!IsValid(this))
				return;
			if (!GetWorld()->GetTimerManager().IsTimerActive(PauseTimer))
				return;
			CurrentTime += 0.1f;
			retTime = std::max(PauseTime - CurrentTime, 0.0f);
			UpdateSafetyFieldValue();
			if (CurrentTime > PauseTime)
			{
				CurrentTime = 0.0f;
				GetWorld()->GetTimerManager().PauseTimer(PauseTimer);
				ShrinkSafetyField();
			}
		},
		0.1f,
		true,
		0.0f
	);
}

void ATeam_AIMagneticField::UpdateSafetyFieldValue()
{
	if (!FMC_SafetyField)
		return;
	//Send To Server
	FLinearColor centerLocToRGB = UKismetMathLibrary::Conv_VectorToLinearColor(SafetySphere->GetComponentLocation());
	float radius = SafetySphere->GetStaticMesh()->GetBoundingBox().GetExtent().X * SafetySphere->GetRelativeScale3D().X * GetActorScale3D().X;
	//Client_ETC
	/*
	{
		centerLocToRGB = release recv server val
		radius = release recv server val
	}
	*/
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), FMC_SafetyField, FName("Location"), centerLocToRGB);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), FMC_SafetyField, FName("Radius"), radius);
	/*auto a = SafetySphere->GetComponentLocation();
	auto b = SafetySphere->GetStaticMesh()->GetBoundingBox().GetExtent().X * SafetySphere->GetRelativeScale3D().X * GetActorScale3D().X;
	*/
	//Server

	auto gm = Cast<ATeam_AIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gm->GetMyPlayer() && gm->GetMyPlayer()->PlayerInfo->object_id() != 1)
		return;
	Protocol::C_SET_MAGNETICFIELD magPkt;
	{
		magPkt.set_r(centerLocToRGB.R);
		magPkt.set_g(centerLocToRGB.G);
		magPkt.set_b(centerLocToRGB.B);
		magPkt.set_a(centerLocToRGB.A);
		magPkt.set_radius(radius);
		magPkt.set_time(retTime);
	}
	GetNetworkManager()->SendPacket(magPkt);
}

void ATeam_AIMagneticField::StartMagnaticField()
{
	PauseSafetyField();
	GetWorld()->GetTimerManager().SetTimer
	(
		DamageTimer,
		[this]() -> void
		{
			if (!IsValid(this))
				return;
			OverlapedActors.Empty();
			TArray<AActor*> inMagneticActors;
			MagneticField->GetOverlappingActors(inMagneticActors);
			for (const auto& iter : inMagneticActors)
				OverlapedActors.Add(iter);

			TArray<AActor*> inSafetyActors;
			SafetySphere->GetOverlappingActors(inSafetyActors);
			for (const auto& iter : inSafetyActors)
				OverlapedActors.Remove(iter);

			for (const auto& iter : OverlapedActors)
				UGameplayStatics::ApplyDamage(iter, Damage, nullptr, this, UDamageType::StaticClass());
		},
		DamageDelay,
		true,
		0.0f
	);
}

void ATeam_AIMagneticField::RecvUpdateSafetyFieldValue(FLinearColor rgb, float radius, float time)
{
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), FMC_SafetyField, FName("Location"), rgb);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), FMC_SafetyField, FName("Radius"), radius);
	retTime = time;
}

UNetworkManager* ATeam_AIMagneticField::GetNetworkManager() const
{
	return GetGameInstance()->GetSubsystem<UNetworkManager>();
}
