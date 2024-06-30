// Fill out your copyright notice in the Description page of Project Settings.


#include "Team_AICharacter_Boss.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Team_AIGameMode.h"
#include "Team_AIController.h"
//#include "Team_AIProjectileBase.h"
#include "Team_AIAnimInstance_Boss.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
//#include "BehaviorTree/BTTaskNode.h"
#include "GameCharacter.h"
//#include "GameFramework/PawnMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

ATeam_AICharacter_Boss::ATeam_AICharacter_Boss()
{
	BossModifyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BossModifyCollision"));
	BossModifyCollision->SetupAttachment(RootComponent);
	RecognizeRange = CreateDefaultSubobject<USphereComponent>(TEXT("RecognizeRange"));
	RecognizeRange->SetupAttachment(RootComponent);
	MeleeAttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollision"));
	MeleeAttackCollision->SetupAttachment(RootComponent);
	MeleeAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Attack4Radius = 500.0f;
}

void ATeam_AICharacter_Boss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATeam_AICharacter_Boss::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	RecognizeRange->OnComponentBeginOverlap.AddDynamic(this, &ATeam_AICharacter_Boss::OnBeginOverlap);
	RecognizeRange->OnComponentEndOverlap.AddDynamic(this, &ATeam_AICharacter_Boss::OnEndOverlap);
	MeleeAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ATeam_AICharacter_Boss::MeleeAttackCollisionBeginOverlap);
}

void ATeam_AICharacter_Boss::BeginPlay()
{
	Super::BeginPlay();
	UKismetMathLibrary::SetRandomStreamSeed(randVar, UKismetMathLibrary::RandomInteger(123456789));
	//AttackRange = MeleeAttackCollision->GetRelativeLocation().X; //NotUse boss
	PlayAudioSystemAtLocation(TEXT("SoundSpawn"), GetActorLocation());
}

bool ATeam_AICharacter_Boss::RecognizePlayers()
{
	bool TargetInRange = false;
	ATeam_AIController* AIController = Cast<ATeam_AIController>(GetController());
	if (!AIController)
		return false;
	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	if (!Blackboard)
		return false;
	AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(ATeam_AIController::TargetKey));
	for (auto iter = Players.CreateIterator(); iter; ++iter)
	{
		if (!(*iter) || Cast<AGameCharacter>(*iter)->IsDead())
		{
			iter.RemoveCurrent();
			continue;
		}
		if (Target && (*iter) == Target)
		{
			TargetInRange = true;
		}
	}
	if (!TargetInRange && Players.Num() > 0)
	{
		Blackboard->SetValueAsObject(ATeam_AIController::TargetKey, Players[UKismetMathLibrary::RandomIntegerInRangeFromStream(0, Players.Num()-1, randVar)]);
	}
	return Players.Num() > 0;
}

int32 ATeam_AICharacter_Boss::GetRandomAttackIdx() const
{
	return AnimInstance->GetAttackMontagesSize() ? UKismetMathLibrary::RandomIntegerInRangeFromStream(0, AnimInstance->GetAttackMontagesSize()-1, randVar) : 0;
}

const TArray<AActor*>& ATeam_AICharacter_Boss::GetRecognizePlayers() const
{
	return Players;
}

void ATeam_AICharacter_Boss::SetMeleeAttackCollision(bool enable)
{
	MeleeAttackCollision->SetCollisionEnabled(enable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
}

void ATeam_AICharacter_Boss::SetAmountKnockback(float value)
{
	AmountKnockback = value;
}

void ATeam_AICharacter_Boss::Attack4()
{
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);
	GetWorld()->OverlapMultiByProfile(OverlapResults, GetActorLocation(), FQuat::Identity, FName("PlayerCheck"), FCollisionShape::MakeSphere(Attack4Radius), CollisionQueryParam);
	if (OverlapResults.Num())
	{
		for (const auto& iter : OverlapResults)
		{
			//UGameplayStatics::ApplyDamage(iter.GetActor(), Attack, GetOwner()->GetInstigatorController(), this, UDamageType::StaticClass());
			UGameplayStatics::ApplyDamage(iter.GetActor(), Attack, nullptr, this, UDamageType::StaticClass());
			//DrawDebugSphere(GetWorld(), GetActorLocation(), Attack4Radius, 16, FColor::Blue, false, 1.0f);
		}
	}
}


void ATeam_AICharacter_Boss::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor)
		return;
	Players.Add(OtherActor);
}

void ATeam_AICharacter_Boss::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor)
		return;
	Players.Remove(OtherActor);
}

void ATeam_AICharacter_Boss::MeleeAttackCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*
	//TODO:BeginOverlap
	if (AttackParticleSystem->Template)
		AttackParticleSystem->Activate(true);
	*/
	FVector dir = OtherActor->GetActorLocation() - GetActorLocation();
	dir.Normalize();
	dir.Z = 0.0f;
	dir = dir * AmountKnockback;
	auto player = Cast<ACharacter>(OtherActor);
	Cast<ACharacter>(OtherActor)->LaunchCharacter(dir, false, false);
	//UGameplayStatics::ApplyDamage(OtherActor, Attack, GetOwner()->GetInstigatorController(), this, UDamageType::StaticClass());
	UGameplayStatics::ApplyDamage(OtherActor, Attack, nullptr, this, UDamageType::StaticClass());

	//TODO : 캐릭터가 밀리는 넉백 패킷
	// boss id ,target id, dir*AmountKnockback
	Protocol::C_AI_KNOCKS_BACK knokbackPkt;
	{
		knokbackPkt.set_object_id(pos.object_id());
		knokbackPkt.set_target_id(Cast<AGameCharacter>(OtherActor)->PlayerInfo->object_id());
		knokbackPkt.set_vx(dir.X);
		knokbackPkt.set_vy(dir.Y);
		knokbackPkt.set_vz(dir.Z);
	}
	GetNetworkManager()->SendPacket(knokbackPkt);
}
