// Fill out your copyright notice in the Description page of Project Settings.


#include "Team_AIProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Team_AIGameMode.h"
#include "GameCharacter.h"
#include "Protocol.pb.h"

// Sets default values
ATeam_AIProjectileBase::ATeam_AIProjectileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	LifeTime = 3.0f;
}

// Called when the game starts or when spawned
void ATeam_AIProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	SetCollisionEnable(false);

	//SphereCollision->OnComponentHit.AddDynamic(this, &ATeam_AIProjectileBase::OnHit);
	/*GetWorld()->GetTimerManager().SetTimer
	(
		LifeTimeHandle,
		[this]() -> void
		{
			if (!IsValid(this))
				return;
			LifeTime -= 0.1f;
			if (LifeTime < 0.0f)
				Destroy();
		},
		0.1f,
		true,
		0.0f
	);*/
}

void ATeam_AIProjectileBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO:BeginOverlap
	auto gm = Cast<ATeam_AIGameMode>(GetWorld()->GetAuthGameMode());
	if (gm->GetMyPlayer()->PlayerInfo->object_id() == 1)
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("ProjectileBeginOverlap!!, %f"), Damage), true, false, FColor::Green);
		UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, this, UDamageType::StaticClass());
	}
	Destroy();
}
/*
void ATeam_AIProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("ProjectileHit!!, %f"), Damage), true, false, FColor::Green);
	UGameplayStatics::
	Damage(OtherActor, Damage, GetOwner()->GetInstigatorController(), this, UDamageType::StaticClass());
	Destroy();
}
*/
// Called every frame
void ATeam_AIProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LifeTime -= DeltaTime;
	if (LifeTime < 0.0f)
		Destroy();
}

void ATeam_AIProjectileBase::ProjectileInitialize(float AttackDamage, float SpeedInit, float SpeedMax, float Gravity)
{
	Damage = AttackDamage;
	ProjectileMovementComponent->InitialSpeed = SpeedInit;
	ProjectileMovementComponent->MaxSpeed = SpeedMax;
	ProjectileMovementComponent->ProjectileGravityScale = Gravity;
}

void ATeam_AIProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ATeam_AIProjectileBase::OnBeginOverlap);
}

void ATeam_AIProjectileBase::SetCollisionEnable(bool bCollide)
{
	SphereCollision->SetCollisionEnabled(bCollide ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
}

void ATeam_AIProjectileBase::BeginDestroy()
{
	Super::BeginDestroy();
	if (GetWorld())
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

