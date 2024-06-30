// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameCharacter.h"
#include "Perception/AISenseConfig_Damage.h"
#include "NetworkManager.h"
#include "Team_AIGameMode.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Capsule Component...?
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;
	//Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	
	//Mesh->SetupAttachment(SphereCollision);
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Bullet Particle"));
	ParticleSystemComponent->SetupAttachment(SphereCollision);
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->InitialSpeed = 2500.f;
	ProjectileMovement->MaxSpeed = 2500.f;
	LifeTime = 1.0f;
}

void AProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);
	//Mesh->OnComponentBeginOverlap.AddDynamic(this,&AProjectile::OnBeginOverlap);
	//Mesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::SetCollisionEnable(bool bCollide)
{
	SphereCollision->SetCollisionEnabled(bCollide ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
}

void AProjectile::BeginDestroy()
{
	Super::BeginDestroy();
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	}
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetCollisionEnable(false);
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

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LifeTime -= DeltaTime;
	if (LifeTime <= 0.f)
	{
		Destroy();
	}
}

void AProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto MyOwner = GetOwner();
	if (MyOwner == nullptr)
	{
		Destroy();
		return;
	}
	auto NetManager = GetGameInstance()->GetSubsystem<UNetworkManager>();
	auto gm = Cast<ATeam_AIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto Player = Cast<AGameCharacter>(MyOwner);

	//if (NetManager->MyPlayer->PlayerInfo->object_id() == 0)
	//if (gm->GetMyPlayer()->PlayerInfo->object_id() == 1)
	//{
		BulletDamage = Cast<AGameCharacter>(MyOwner)->GetDamage();
		auto MyOwnerInstigator = MyOwner->GetInstigatorController();
		auto DamageTypeClass = UDamageType::StaticClass();
		if (OtherActor != nullptr && OtherActor != this && OtherActor != MyOwner)
		{
			//UGameplayStatics::ApplyDamage(OtherActor, BulletDamage, MyOwnerInstigator, Player, DamageTypeClass);
			UGameplayStatics::ApplyDamage(OtherActor, BulletDamage, nullptr, Player, DamageTypeClass);
			//AGameCharacter* HitPlayer = Cast<AGameCharacter>(OtherActor);
			//if (IsValid(HitPlayer) == false)
			//{
			//	Destroy();
			//	return;
			//}
			// 
			//Protocol::C_HIT HitPkt;
			//{
			//	HitPkt.set_ownerid(Player->PlayerInfo->object_id());
			//	HitPkt.set_targetid(HitPlayer->PlayerInfo->object_id());
			//	HitPkt.set_damage(BulletDamage);
			//
			//	GetNetworkManager()->SendPacket(HitPkt);
			//}
		}
	//}
	UAISense_Damage::ReportDamageEvent(GetWorld(), OtherActor, GetOwner(), BulletDamage, GetActorLocation(), GetActorLocation());
	Destroy();
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	auto MyOwner = GetOwner();
	if (MyOwner == nullptr)
	{
		Destroy();
		return;
	}
	auto NetManager = GetGameInstance()->GetSubsystem<UNetworkManager>();
	auto gm = Cast<ATeam_AIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto Player = Cast<AGameCharacter>(MyOwner);

	//if (NetManager->MyPlayer->PlayerInfo->object_id() == 0)
	if (gm->GetMyPlayer()->PlayerInfo->object_id() == 1)
	{
		//아마 여기서 충돌 했다고 패킷을 보내야 겠지?
		BulletDamage = Cast<AGameCharacter>(MyOwner)->GetDamage();
		auto MyOwnerInstigator = MyOwner->GetInstigatorController();
		auto DamageTypeClass = UDamageType::StaticClass();
		if (OtherActor != nullptr && OtherActor != this && OtherActor != MyOwner)
		{
			//UGameplayStatics::ApplyDamage(OtherActor, BulletDamage, MyOwnerInstigator, this, DamageTypeClass);
			UGameplayStatics::ApplyDamage(OtherActor, BulletDamage, nullptr, this, DamageTypeClass);
			AGameCharacter* HitPlayer = Cast<AGameCharacter>(OtherActor);
			if (IsValid(HitPlayer) == false)
			{
				Destroy();
				return;
			}
			Protocol::C_HIT HitPkt;
			{
				HitPkt.set_ownerid(Player->PlayerInfo->object_id());
				HitPkt.set_targetid(HitPlayer->PlayerInfo->object_id());
				HitPkt.set_damage(BulletDamage);

				GetNetworkManager()->SendPacket(HitPkt);
			}
		}
	}

	UAISense_Damage::ReportDamageEvent(GetWorld(), OtherActor, GetOwner(), BulletDamage, GetActorLocation(), GetActorLocation());
	Destroy();
}

UNetworkManager* AProjectile::GetNetworkManager() const
{
	return GetGameInstance()->GetSubsystem<UNetworkManager>();
}
