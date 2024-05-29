// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "NetworkManager.h"
#include "GameCharacter.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(RootComponent);
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(StaticMesh);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::SetDamage(float Damage)
{
	WeaponDamage = Damage;
}

void AWeapon::Shot()
{
	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Location, Rotation);
	if (!Projectile)
		return;
	auto MyOwner = GetOwner();
	Projectile->SetOwner(MyOwner);
	Projectile->SetCollisionEnable(true);

	Protocol::C_FIRE firePkt;
	{
		AGameCharacter* my = Cast<AGameCharacter>(MyOwner);
		Protocol::ProjectileInfo* projInfo = firePkt.mutable_projectile();
		projInfo->set_object_id(my->PlayerInfo->object_id());
		projInfo->set_x(Location.X);
		projInfo->set_y(Location.Y);
		projInfo->set_z(Location.Z);
		projInfo->set_yaw(Rotation.Yaw);
		projInfo->set_pitch(Rotation.Pitch);
		projInfo->set_roll(Rotation.Roll);
		projInfo->set_bfight(my->IsFight());
		GetNetworkManager()->SendPacket(firePkt);
	}
}

UNetworkManager* AWeapon::GetNetworkManager() const
{
	return GetGameInstance()->GetSubsystem<UNetworkManager>();
}

TSubclassOf<class AProjectile> AWeapon::GetPrjClass()
{
	return ProjectileClass;
}
