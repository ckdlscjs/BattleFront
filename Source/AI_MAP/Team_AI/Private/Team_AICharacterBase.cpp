// Fill out your copyright notice in the Description page of Project Settings.


#include "Team_AICharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Team_AIGameMode.h"
#include "Team_AIController.h"
//#include "Team_AIProjectileBase.h"
#include "Team_AIAnimInstance.h"
#include "Team_AIGameInstance.h"
#include "Team_AIPatrolRoute.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GameCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
// Sets default values
ATeam_AICharacterBase::ATeam_AICharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = ATeam_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	/*
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SKM_MESH(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny"));
	if (SKM_MESH.Succeeded())
		GetMesh()->SetSkeletalMesh(SKM_MESH.Object);
	GetMesh()->SetRelativeRotation({ 0.0f, -90.0f, 0.0f });*/
	TargetTag = "PlayerController";
	AttackRange = 500.0f;
	AttackSpeed = 1.0f;
	AttackDelay = 3.0f;
	DetectRadius = 1000.0f;
	PatrolRadius = 3000.0f;
	ProjectileSpeed = 300.0f;
	Level = 1;
	EnemyStatDecreaseAmount = 0.3f;
	/*
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);
	*/
	PatrolIndex = 0;
	PatrolDirection = 1;
	CurrentState = ECharacterState::IDLE;
	AttackParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("AttackParticleSystem"));
	AttackParticleSystem->SetupAttachment(RootComponent);
	AttackParticleSystem->bAutoActivate = false;
}

// Called when the game starts or when spawned
void ATeam_AICharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	//TODO : GetGameLevel
	SetStat(Cast<ATeam_AIGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->GetGameLevel());
	ATeam_AIController* AIController = Cast<ATeam_AIController>(GetController());
	if (!AIController)
		return;
	AIController->SetState(ECharacterState::IDLE);
}

void ATeam_AICharacterBase::TakenDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("TakenDamage:%f"), Damage));
	CurHp -= Damage;
	if (CurHp <= 0.0f && !AnimInstance->GetDead())
	{
		SendTest("Dead");
		BehaviorDead();
	}
		
}

void ATeam_AICharacterBase::SetStat(int32 CharacterLevel)
{
	auto GameInstance = Cast<UTeam_AIGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GameInstance)
		return;
	FTeam_CharacterData* RowData = GameInstance->GetCharacterRowData(CharacterLevel);
	//TODO : SetRowData
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("CharacterLevel : %d"), RowData->Level));
	Level = RowData->Level;
	MaxHP = RowData->MaxHP * EnemyStatDecreaseAmount;
	CurHp = MaxHP;
	Attack = RowData->Attack * EnemyStatDecreaseAmount;
	Armor = RowData->Armor * EnemyStatDecreaseAmount;
	Speed = RowData->Speed * EnemyStatDecreaseAmount;
}



// Called every frame
void ATeam_AICharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SpeedCurrent = GetVelocity().Size();
}

void ATeam_AICharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (!IsValid(GetMesh()))
		return;
	AnimInstance = Cast<UTeam_AIAnimInstance>(GetMesh()->GetAnimInstance());
	if (!AnimInstance)
		return;
	OnTakeAnyDamage.AddDynamic(this, &ATeam_AICharacterBase::TakenDamage);
	/*
	AnimInstance->OnAttackStart.AddLambda([this]() -> void 
		{
			UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("LambdaAttack!")));
			if (ProjectileClass)
			{
				ATeam_AIProjectileBase* Projectile = Cast<ATeam_AIProjectileBase>(GetWorld()->SpawnActor<AActor>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), GetActorRotation()));
				Projectile->SetOwner(this);
				Projectile->ProjectileInitialize(Attack, ProjectileSpeed, ProjectileSpeed, 0.0f);
				Projectile->SetActorEnableCollision(true);
				//Projectile->SetActorLocation(ProjectileSpawnPoint->GetComponentLocation());
			}
		});
	*/
	AnimInstance->OnDead.AddLambda([this]() -> void
		{
			UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("LambdaDead!!!!!!!!!!!!!!!!!!!!!")));
			Cast<ATeam_AIGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->DeleteSpawnActor(this);
			Destroy();
		});
	AnimInstance->OnAttackMontageEnd.AddLambda([this]() -> void
		{
			ATeam_AIController* AIController = Cast<ATeam_AIController>(GetController());
			if (!AIController)
				return;
			UBehaviorTreeComponent* BehaviorTree = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent());
			if (!BehaviorTree)
				return;
			BehaviorTree->OnTaskFinished(Cast<UBTTaskNode>(BehaviorTree->GetActiveNode()), EBTNodeResult::Succeeded);
		});
}

// Called to bind functionality to input
void ATeam_AICharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

FName ATeam_AICharacterBase::GetTargetTag() const
{
	return TargetTag;
}

float ATeam_AICharacterBase::GetAttackRange() const
{
	return AttackRange;
}

float ATeam_AICharacterBase::GetAttackDelay() const
{
	return AttackDelay;
}

/*
* TODO:Animation
*/
void ATeam_AICharacterBase::BehaviorAttack()
{
	if (IsDead())
		return;
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Attacking!!")), true, false, FColor::Red);
	//TODO : Attak
	if (AnimInstance)
		AnimInstance->PlayAttackMontage(AttackSpeed);
}

void ATeam_AICharacterBase::BehaviorDead()
{
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Dead!!")), true, false, FColor::Red);
	//TODO : 사망
	if (AnimInstance)
		AnimInstance->PlayDeadMontage(AttackSpeed);
	ATeam_AIController* AIController = Cast<ATeam_AIController>(GetOwner());
	if (!AIController)
		return;
	UBehaviorTreeComponent* BehaviorTree = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent());
	if (!BehaviorTree)
		return;
	BehaviorTree->StopTree();
}


void ATeam_AICharacterBase::OnDeadMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("LambdaDead!!!!!!!!!!!!!!!!!!!!!")));
	Destroy();
}

ATeam_AIPatrolRoute* ATeam_AICharacterBase::GetPatrolRoute() const
{
	return PatrolRoute;
}

void ATeam_AICharacterBase::SetPatrolRoute(ATeam_AIPatrolRoute* Route)
{
	PatrolRoute = Route;
}

int32 ATeam_AICharacterBase::GetCurrentPatrolIndex() const
{
	return PatrolIndex;
}

int32 ATeam_AICharacterBase::GetCurrentPatrolDirection() const
{
	return PatrolDirection;
}

void ATeam_AICharacterBase::IncrementPatrolIndex()
{
	if (PatrolIndex >= PatrolRoute->GetSizeOfPatrolRoute() - 1)
		PatrolDirection = -1;
	PatrolIndex += PatrolDirection;
	if (PatrolIndex < 0)
	{
		PatrolIndex = 0;
		PatrolDirection = 1;
	}
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("%d"), PatrolIndex));
}

void ATeam_AICharacterBase::SetCurrentState(ECharacterState State)
{
	CurrentState = State;
}

ECharacterState ATeam_AICharacterBase::GetCurrentState()
{
	return CurrentState;
}

void ATeam_AICharacterBase::SendTest(FString str)
{
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("%s"), *str));
	if (RecvAI)
		RecvAI->RecvTest(str);
	//To NetworkManger 
	//switch
}

void ATeam_AICharacterBase::RecvTest(FString str)
{
}

float ATeam_AICharacterBase::GetDetectRadius()
{
	return DetectRadius;
}

float ATeam_AICharacterBase::GetPatrolRadius()
{
	return PatrolRadius;
}

float ATeam_AICharacterBase::GetCurrentSpeed()
{
	return SpeedCurrent;
}

bool ATeam_AICharacterBase::IsDead() const
{
	return AnimInstance ? AnimInstance->GetDead() : false;
}

bool ATeam_AICharacterBase::IsAttacking() const
{
	return AnimInstance ? AnimInstance->GetAttacking() : false;
}

