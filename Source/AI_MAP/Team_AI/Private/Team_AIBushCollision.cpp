// Fill out your copyright notice in the Description page of Project Settings.


#include "Team_AIBushCollision.h"
#include "Components/BoxComponent.h"
#include "CharacterController.h"
#include "GameCharacter.h"
#include "Components/SkeletalMeshComponent.h"
// Sets default values
ATeam_AIBushCollision::ATeam_AIBushCollision()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	PlayerInBush = false;
}

void ATeam_AIBushCollision::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ATeam_AIBushCollision::BushBeginOverlap);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ATeam_AIBushCollision::BushEndOverlap);
}

// Called when the game starts or when spawned
void ATeam_AIBushCollision::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATeam_AIBushCollision::BushBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto overlapActor = Cast<AGameCharacter>(OtherActor);
	Players.Add(overlapActor);
	if (Cast<ACharacterController>(overlapActor->GetController()) == Cast<ACharacterController>(GetWorld()->GetFirstPlayerController()))
		PlayerInBush = true;
	for (const auto& iter : Players)
	{
		Cast<AGameCharacter>(iter)->SetVisibility(PlayerInBush);
	}
}

void ATeam_AIBushCollision::BushEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto overlapActor = Cast<AGameCharacter>(OtherActor);
	Players.Remove(overlapActor);
	overlapActor->SetVisibility(true);
	if (Cast<ACharacterController>(overlapActor->GetController()) == Cast<ACharacterController>(GetWorld()->GetFirstPlayerController()))
		PlayerInBush = false;
	for (const auto& iter : Players)
	{
		Cast<AGameCharacter>(iter)->SetVisibility(PlayerInBush);
	}
}

// Called every frame
void ATeam_AIBushCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

