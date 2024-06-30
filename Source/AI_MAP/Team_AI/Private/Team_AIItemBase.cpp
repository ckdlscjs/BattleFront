// Fill out your copyright notice in the Description page of Project Settings.


#include "Team_AIItemBase.h"
#include "Components/BoxComponent.h"
#include "GameCharacter.h"
#include "Particles/ParticleSystemComponent.h"
#include "Materials/MaterialInstance.h"
#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"
// Sets default values
ATeam_AIItemBase::ATeam_AIItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);
	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleSystem->SetupAttachment(RootComponent);
	ItemAbilityInfo = 0.0f;
	MeshDissolveTime = 1.5f;
	DissolveTimeCurrent = 0.0f;
	ItemValue = 0.f;
	rotTime2Pi = 2.0f;
}

// Called when the game starts or when spawned
void ATeam_AIItemBase::BeginPlay()
{
	Super::BeginPlay();
	ParticleSystem->bAutoActivate = false;
	MaterialDynamicInst = StaticMesh->CreateDynamicMaterialInstance(0, StaticMesh->GetMaterial(0));
	MaterialDynamicInst->SetScalarParameterValue(FName("GlowValue"), 300.0f);
	for (const auto& iter : K2_GetComponentsByClass(UAudioComponent::StaticClass()))
		AudioSystems.Add(TTuple<FString, UAudioComponent*>(iter->GetName(), Cast<UAudioComponent>(iter)));
}

void ATeam_AIItemBase::ItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto player = Cast<AGameCharacter>(OtherActor);
	if (!player)
		return;
	if(ParticleSystem->Template)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem->Template, player->GetActorLocation());
	PlayAudioSystemAtLocation(TEXT("GetItem"), GetActorLocation());
	//Clinet Only
	GetItemAbility(player, ItemAbilityInfo);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetWorldTimerManager().SetTimer
	(
		ItemTimerHandle,
		[this]() -> void
		{
			if (!IsValid(this))
				return;
			float ratio = 1.0f - (DissolveTimeCurrent / MeshDissolveTime);
			//material translucen
			MaterialDynamicInst->SetScalarParameterValue(FName("Ratio_Dissolve"), ratio);
			MaterialDynamicInst->SetScalarParameterValue(FName("Ratio_Intensity"), ratio);

			DissolveTimeCurrent += GetWorld()->GetDeltaSeconds();
			if (DissolveTimeCurrent > MeshDissolveTime)
			{
				GetWorldTimerManager().PauseTimer(ItemTimerHandle);
				Destroy();
			}
		},
		GetWorld()->GetDeltaSeconds(),
		true,
		0.0f
	);
}

// Called every frame
void ATeam_AIItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	rotTimeCur += DeltaTime;
	if (rotTimeCur > rotTime2Pi)
		rotTimeCur -= rotTime2Pi;
	float ratio = rotTimeCur / rotTime2Pi;
	StaticMesh->SetRelativeRotation(FRotator(0.0f, 360 * ratio, 0.0f));
	StaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, std::sin(360 * ratio * PI * 1/180.0f)) * 10.0f);
}

void ATeam_AIItemBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ATeam_AIItemBase::ItemBeginOverlap);
}

void ATeam_AIItemBase::BeginDestroy()
{
	Super::BeginDestroy();
	if (GetWorld())
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void ATeam_AIItemBase::PlayAudioSystem(FString str)
{
	if (!AudioSystems.Find(str))
		return;
	UAudioComponent* AudioComponent = AudioSystems[str];
	if (!AudioComponent)
		return;
	if (!AudioComponent->Sound)
		return;
	AudioComponent->Play();
}

void ATeam_AIItemBase::PlayAudioSystemAtLocation(FString str, FVector Loc)
{
	if (!AudioSystems.Find(str))
		return;
	UAudioComponent* AudioComponent = AudioSystems[str];
	if (!AudioComponent)
		return;
	if (!AudioComponent->Sound)
		return;
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), AudioComponent->Sound, Loc);
}

