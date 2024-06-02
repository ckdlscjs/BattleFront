// Fill out your copyright notice in the Description page of Project Settings.


#include "Team_AICharacterBase.h"
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
#include "NetworkManager.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Team_AIGameMode.h"
#include "Materials/MaterialInstance.h"

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
	AnimationSpeed = 1.0f;
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
	//AttackParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("AttackParticleSystem"));
	//AttackParticleSystem->SetupAttachment(RootComponent);
	//AttackParticleSystem->bAutoActivate = false;
	HitOverlayTime = 1.0f;
}

// Called when the game starts or when spawned
void ATeam_AICharacterBase::BeginPlay()
{
	Super::BeginPlay();
	for (const auto& iter : K2_GetComponentsByClass(UParticleSystemComponent::StaticClass()))
		ParticleSystems.Add(TTuple<FString, UParticleSystemComponent*>(iter->GetName(), Cast<UParticleSystemComponent>(iter)));
	BehviorSpawn();
	//TODO : GetGameLevel
	SetStat(Cast<ATeam_AIGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->GetGameLevel());
	ATeam_AIController* AIController = Cast<ATeam_AIController>(GetController());
	if (!AIController)
		return;
	AIController->SetState(ECharacterState::IDLE);
}

void ATeam_AICharacterBase::TakenDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (HitOverlayMaterial)
		GetMesh()->SetOverlayMaterial(HitOverlayMaterial);

	FTimerHandle OverlayHandle;
	GetWorldTimerManager().SetTimer
	(
		OverlayHandle,
		[&]() -> void
		{
			GetMesh()->SetOverlayMaterial(nullptr);
			GetWorldTimerManager().ClearTimer(OverlayHandle);
		},
		HitOverlayTime,
		false
	);

	if (GetNetworkManager()->GameMode->GetMyPlayer()->PlayerInfo->object_id() == 1)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("TakenDamage:%f"), Damage));
		CurHp -= Damage;

	
		Protocol::C_AIDAMAGED AIDmgedPkt;
		{
			AIDmgedPkt.set_object_id(pos.object_id());
			AIDmgedPkt.set_hp(CurHp);

			GetNetworkManager()->SendPacket(AIDmgedPkt);
		}
		if (CurHp <= 0.0f && !AnimInstance->GetDead())
		{
			Protocol::C_AIDEAD AIDeadPkt;
			{
				AIDeadPkt.set_object_id(pos.object_id());

				GetNetworkManager()->SendPacket(AIDeadPkt);
			}

			//SendTest("Dead");
			BehaviorDead();
		}
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
	//CurHp = MaxHP;
	CurHp = 1.0f;
	Attack = RowData->Attack * EnemyStatDecreaseAmount;
	Armor = RowData->Armor * EnemyStatDecreaseAmount;
	Speed = RowData->Speed * EnemyStatDecreaseAmount;
}

// Called every frame
void ATeam_AICharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//auto player = GetNetworkManager()->gamedmoMyPlayer;
	auto player = GetNetworkManager()->GameMode->GetMyPlayer();
	if (!player)
		return;
	if(player->PlayerInfo->object_id() == 1)
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
			auto gm = Cast<ATeam_AIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			gm->SetAIDespawn(pos.object_id());
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
	AnimInstance->OnSpawnParticle.AddLambda([this]() -> void
		{
			ActivateParticleSystem(TEXT("Spawn"));
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
void ATeam_AICharacterBase::BehaviorAttack(int idx)
{
	if (IsDead())
		return;
	//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Attacking!!")), true, false, FColor::Red);
	//TODO : Attak
	if (AnimInstance)
		AnimInstance->PlayAttackMontage(AnimationSpeed, idx);
}

void ATeam_AICharacterBase::BehaviorDead()
{
	//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Dead!!")), true, false, FColor::Red);
	//TODO : 사망
	if (AnimInstance)
		AnimInstance->PlayDeadMontage(AnimationSpeed);
	ATeam_AIController* AIController = Cast<ATeam_AIController>(GetOwner());
	if (!AIController)
		return;
	UBehaviorTreeComponent* BehaviorTree = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent());
	if (!BehaviorTree)
		return;
	BehaviorTree->StopTree();
}


void ATeam_AICharacterBase::BehviorSpawn()
{
	if (IsDead())
		return;
	if (AnimInstance)
		if(AnimInstance->PlaySpawnMontage(AnimationSpeed) < 0)
			ActivateParticleSystem(TEXT("Spawn"));
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

void ATeam_AICharacterBase::SendTest(FString str, int idx)
{
	if (str == "MoveTo")
	{
		Protocol::C_AIMOVE aimovePkt;
		{
			FVector loc = GetActorLocation();
			FRotator rot = GetActorRotation();
			Protocol::PosInfo* Info = aimovePkt.mutable_info();
			Info->set_object_id(pos.object_id());

			Info->set_x(loc.X);
			Info->set_y(loc.Y);
			Info->set_z(loc.Z);

			Info->set_yaw(rot.Yaw);
			Info->set_speed(SpeedCurrent);

			GetNetworkManager()->SendPacket(aimovePkt);
		}
	}
	else if (str == "MoveEnd")
	{
		Protocol::C_AIMOVESTOP aimovePkt;
		{
			aimovePkt.set_object_id(pos.object_id());
			aimovePkt.set_speed(pos.speed());

			GetNetworkManager()->SendPacket(aimovePkt);
		}
	}
	else if (str == "Attack")
	{
		Protocol::C_AIATTACK aiAttackPkt;
		{
			aiAttackPkt.set_object_id(pos.object_id());
			aiAttackPkt.set_attack_idx(idx);
			GetNetworkManager()->SendPacket(aiAttackPkt);
		}
	}
	else if (str == "RotateTo")
	{
		Protocol::C_AIROTATE airotPkt;
		{
			airotPkt.set_object_id(pos.object_id());

			FRotator rot = GetActorRotation();
			airotPkt.set_yaw(rot.Yaw);
			airotPkt.set_pitch(rot.Pitch);
			airotPkt.set_roll(rot.Roll);

			FVector loc = GetActorLocation();
			airotPkt.set_x(loc.X);
			airotPkt.set_y(loc.Y);
			airotPkt.set_z(loc.Z);

			GetNetworkManager()->SendPacket(airotPkt);
		}
	}
	//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("%s"), *str));
	//if (RecvAI)
	//	RecvAI->RecvTest(str);
	//To NetworkManger 
	//switch
}

void ATeam_AICharacterBase::RecvTest(FString str)
{
}

void ATeam_AICharacterBase::ActivateParticleSystem(FString str)
{
	UParticleSystemComponent* particleSystem = ParticleSystems[str];
	if (!particleSystem)
		return;
	if (!particleSystem->Template)
		return;
	particleSystem->Activate(true);
}

const UParticleSystemComponent* ATeam_AICharacterBase::GetParticleSystemComponent(FString str) const
{
	return ParticleSystems[str];
}

void ATeam_AICharacterBase::AttackParticletoActors(FString str)
{
	for (const auto& actor : Players)
	{
		UGameplayStatics::ApplyDamage(actor, Attack, GetOwner()->GetInstigatorController(), this, UDamageType::StaticClass());
		if (!str.IsEmpty() && ParticleSystems[str]->Template)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystems[str]->Template, actor->GetActorLocation(), ParticleSystems[str]->GetRelativeRotation(), ParticleSystems[str]->GetRelativeScale3D());
	}
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

void ATeam_AICharacterBase::SetCurrentSpeed(float speed)
{
	SpeedCurrent = speed;
}

bool ATeam_AICharacterBase::IsDead() const
{
	return AnimInstance ? AnimInstance->GetDead() : false;
}

bool ATeam_AICharacterBase::IsAttacking() const
{
	return AnimInstance ? AnimInstance->GetAttacking() : false;
}

UNetworkManager* ATeam_AICharacterBase::GetNetworkManager() const
{
	return GetGameInstance()->GetSubsystem<UNetworkManager>();
}

void ATeam_AICharacterBase::SetCurHP(float hp)
{
	CurHp = hp;
}
