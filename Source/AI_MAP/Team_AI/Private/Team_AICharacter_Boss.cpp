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
//#include "GameCharacter.h"
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
	Amount_Knockback1 = 0.0f;
	Amount_Knockback3 = 5000.0f;
	Attack4Delay = 1.0f;
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
	Cast<UTeam_AIAnimInstance_Boss>(AnimInstance)->OnAttack1Start.AddLambda([this]() -> void
		{
			AmountKnockback = Amount_Knockback1;
			MeleeAttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		});
	Cast<UTeam_AIAnimInstance_Boss>(AnimInstance)->OnAttack1End.AddLambda([this]() -> void
		{
			AmountKnockback = 0.0f;
			MeleeAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);;
		});

	Cast<UTeam_AIAnimInstance_Boss>(AnimInstance)->OnAttack2Start.AddLambda([this]() -> void
		{
			for (const auto& actor : Players)
			{
				UGameplayStatics::ApplyDamage(actor, Attack, GetOwner()->GetInstigatorController(), this, UDamageType::StaticClass());
			}
		});
	//Cast<UTeam_AIAnimInstance_Boss>(AnimInstance)->OnAttack2End.AddLambda([this]() -> void
	//	{
	//		MeleeAttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//	});

	Cast<UTeam_AIAnimInstance_Boss>(AnimInstance)->OnAttack3Start.AddLambda([this]() -> void
		{
			AmountKnockback = Amount_Knockback3;
			MeleeAttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		});
	Cast<UTeam_AIAnimInstance_Boss>(AnimInstance)->OnAttack3End.AddLambda([this]() -> void
		{
			AmountKnockback = 0.0f;
			MeleeAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		});

	Cast<UTeam_AIAnimInstance_Boss>(AnimInstance)->OnAttack4Start.AddLambda([this]() -> void
		{
			GetWorldTimerManager().SetTimer
			(
				BossAttackTimerHandle,
				[this]() -> void
				{
					TArray<FOverlapResult> OverlapResults;
					FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);

					GetWorld()->OverlapMultiByProfile(OverlapResults, GetActorLocation(), FQuat::Identity, FName("PlayerCheck"), FCollisionShape::MakeSphere(Attack4Radius), CollisionQueryParam);
					if (OverlapResults.Num())
					{
						for (const auto& iter : OverlapResults)
						{
							ActivateParticleSystem(TEXT("Attack4"));
							UKismetSystemLibrary::PrintString(GetWorld(), FString(iter.GetActor()->GetName()), true, false);
							DrawDebugLine(GetWorld(), GetActorLocation(), iter.GetActor()->GetActorLocation(), FColor::Red, false, 0.3f);
							DrawDebugSphere(GetWorld(), GetActorLocation(), Attack4Radius, 16, FColor::Blue, false, 1.0f);
						}
					}
				},
				Attack4Delay,
				true,
				0.0f
			);			
		});
	Cast<UTeam_AIAnimInstance_Boss>(AnimInstance)->OnAttack4End.AddLambda([this]() -> void
		{
			GetWorldTimerManager().ClearTimer(BossAttackTimerHandle);
		});


	/*Cast<UTeam_AIAnimInstance_Boss>(AnimInstance)->OnAttack5Start.AddLambda([this]() -> void
		{
			Attack4Scale = 0.0f;

		});
	Cast<UTeam_AIAnimInstance_Boss>(AnimInstance)->OnAttack5End.AddLambda([this]() -> void
		{
			Attack4Scale = 0.0f;

		});*/

	Cast<UTeam_AIAnimInstance_Boss>(AnimInstance)->OnAttack2Particle.AddLambda([this]() -> void
		{
			const UParticleSystemComponent* particleSystem = GetParticleSystemComponent(TEXT("Attack2"));
			if (!particleSystem)
				return;
			for (const auto& actor : Players)
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particleSystem->Template, actor->GetActorLocation(), particleSystem->GetRelativeRotation(), particleSystem->GetRelativeScale3D());
		});
	Cast<UTeam_AIAnimInstance_Boss>(AnimInstance)->OnAttack3Particle.AddLambda([this]() -> void
		{
			ActivateParticleSystem(TEXT("Attack3"));
		});

}

void ATeam_AICharacter_Boss::BeginPlay()
{
	Super::BeginPlay();
	UKismetMathLibrary::SetRandomStreamSeed(randVar, UKismetMathLibrary::RandomInteger(123456789));
	//AttackRange = MeleeAttackCollision->GetRelativeLocation().X; //NotUse boss
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
		if (!(*iter))
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
	Cast<ACharacter>(OtherActor)->LaunchCharacter(dir*AmountKnockback, false, false);
	UGameplayStatics::ApplyDamage(OtherActor, Attack, GetOwner()->GetInstigatorController(), this, UDamageType::StaticClass());
}
