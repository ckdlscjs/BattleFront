// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameCharacter.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Components/SphereComponent.h"
// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Capsule Component...?
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = SphereCollision;
	Mesh->SetupAttachment(SphereCollision);
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->InitialSpeed = 1000.0f;
	ProjectileMovement->MaxSpeed = 1000.0f;
}

void AProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//Mesh->OnComponentBeginOverlap.AddDynamic(this,&AProjectile::OnBeginOverlap);
	//SphereCollision->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);
}

void AProjectile::SetCollisionEnable(bool bCollide)
{
	SphereCollision->SetCollisionEnabled(bCollide? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetCollisionEnable(false);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto MyOwner = GetOwner();
	if (MyOwner == nullptr)
	{
		Destroy();
		return;
	}
	BulletDamage = Cast<AGameCharacter>(MyOwner)->GetDamage();
	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();
	if (OtherActor != nullptr && OtherActor != this && OtherActor != MyOwner)
	{
		UGameplayStatics::ApplyDamage(OtherActor, BulletDamage, MyOwnerInstigator, this, DamageTypeClass);
	}
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
	BulletDamage = Cast<AGameCharacter>(MyOwner)->GetDamage();
	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();
	if (OtherActor != nullptr && OtherActor != this && OtherActor != MyOwner)
	{
		UGameplayStatics::ApplyDamage(OtherActor, BulletDamage, MyOwnerInstigator, this, DamageTypeClass);
	}
	UAISense_Damage::ReportDamageEvent(GetWorld(), OtherActor, GetOwner(), BulletDamage, GetActorLocation(), GetActorLocation());
	Destroy();
}

