// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityBomb.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameCharacter.h"
#include "Protocol.pb.h"
#include "NetworkManager.h"

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
	CapsuleCompoent->SetGenerateOverlapEvents(true);
}

void AAbilityBomb::ProjectileBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FVector Loc = GetActorLocation();
	Loc.Z = 0.f;
	if (HitParticles)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitParticles, Loc, GetActorRotation());
	}
	if (BombSound)
	{
		PlaySound(BombSound);
	}
	AGameCharacter* MyOwner = Cast<AGameCharacter>(GetOwner());
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, Loc, 200.f, nullptr, TArray<AActor*>(), MyOwner, false, true);
	Destroy();
}

void AAbilityBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AAbilityBomb::SetLocation(FVector& Location, int abilityIdx)
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
	//CapsuleCompoent->SetGenerateOverlapEvents(true);

	Protocol::C_PLAYERSKILL_BOMB bombPkt;
	{
		auto player = Cast<AGameCharacter>(GetOwner());
		bombPkt.set_object_id(player->PlayerInfo->object_id());
		bombPkt.set_abilityarrayidx(abilityIdx);
		bombPkt.set_x(Location.X);
		bombPkt.set_y(Location.Y);
		bombPkt.set_z(Location.Z);
		bombPkt.set_dmg(this->GetAbilityDetail());
	}
	GetNetworkManager()->SendPacket(bombPkt);
}

void AAbilityBomb::AbilityLevelUp()
{
	Super::AbilityLevelUp();
	Damage = (MyAbilityLevel + 5) * 2;
	ProjectileCount++;
}

int32 AAbilityBomb::GetProjCount()
{
	return ProjectileCount;
}

float AAbilityBomb::GetAbilityDetail()
{
	return Damage;
}

void AAbilityBomb::SetAbilityDetail(float Details)
{
	Damage = Details;
}

void AAbilityBomb::PlaySound(USoundBase* Sound)
{
	FVector Loc = GetActorLocation();
	Loc.Z = 0.f;
	if (Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, Sound, Loc);
	}
}

