// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityGranade.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h" 
#include "Kismet/KismetMathLibrary.h"
#include "GrenadeFireArea.h"
#include "Kismet/GameplayStatics.h"
#include "AI_MAP.h"
#include "Protocol.pb.h"
#include "NetworkManager.h"
#include "GameCharacter.h"	
AAbilityGranade::AAbilityGranade()
{

	CapsuleCompoent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCompoent"));
	RootComponent = CapsuleCompoent;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	StaticMesh->SetupAttachment(RootComponent);
	MyAbilityLevel = 0;
	Damage = MyAbilityLevel + 6;
	CoolTime = 7.f - MyAbilityLevel;
	ProjectileCount = 1;
	Type = AbilityType::Range;
	CapsuleCompoent->SetGenerateOverlapEvents(false);

}

void AAbilityGranade::BeginPlay()
{
	Super::BeginPlay();
	CapsuleCompoent->OnComponentBeginOverlap.AddDynamic(this, &AAbilityGranade::ProjectileBeginOverlap);

}

void AAbilityGranade::ProjectileBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GrenadeSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, GrenadeSound, GetActorLocation());
	}
	CapsuleCompoent->SetGenerateOverlapEvents(false);
	FVector Loc = SweepResult.Location;
	Loc.Z = 0;
	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, Loc, GetActorRotation());
	}
	
	AGrenadeFireArea* FireArea = GetWorld()->SpawnActor<AGrenadeFireArea>(FireAreaClass, Loc,FRotator::ZeroRotator);
	FireArea->SetFireDamage(Damage);
	auto MyOwner = GetOwner();
	FireArea->SetOwner(MyOwner);
	Destroy();
}

void AAbilityGranade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AAbilityGranade::SetLocation(FVector& Location, int abilityIdx)
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
	SetForce(RandVec);
	
	//Server
	Protocol::C_PLAYERSKILL_GRANADE granPkt;
	{
		auto player = Cast<AGameCharacter>(GetOwner());
		granPkt.set_object_id(player->PlayerInfo->object_id());
		granPkt.set_abilityarrayidx(abilityIdx);
		granPkt.set_x(Location.X);
		granPkt.set_y(Location.Y);
		granPkt.set_z(Location.Z);
		granPkt.set_rx(RandVec.X);
		granPkt.set_ry(RandVec.Y);
		granPkt.set_rz(RandVec.Z);
		granPkt.set_dmg(this->GetAbilityDetail());
	}
	GetNetworkManager()->SendPacket(granPkt);
}

void AAbilityGranade::AbilityLevelUp()
{
	Super::AbilityLevelUp();
	Damage = MyAbilityLevel + 6;
}

int32 AAbilityGranade::GetProjCount()
{
	return ProjectileCount;
}

float AAbilityGranade::GetAbilityDetail()
{
	return Damage;
}

void AAbilityGranade::SetAbilityDetail(float Details)
{
	Damage = Details;
}
void AAbilityGranade::SetForce(FVector force)
{
	CapsuleCompoent->SetGenerateOverlapEvents(true);
	ProjectileMovement->AddForce(force * 20000);
}
