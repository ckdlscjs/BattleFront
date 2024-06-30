// Fill out your copyright notice in the Description page of Project Settings.


#include "Team_AIPatrolRoute.h"
#include "Components/SplineComponent.h"
#include "Components/BoxComponent.h"
#include "Team_AISpawnPointPatrol.h"
#include "Team_AICharacterBase.h"
#include "Team_AIController.h"
#include "BehaviorTree/BehaviorTree.h"
//#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
//#include "BehaviorTree/BTTaskNode.h"
// Sets default values
ATeam_AIPatrolRoute::ATeam_AIPatrolRoute()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PatrolRoute = CreateDefaultSubobject<USplineComponent>(TEXT("PatrolRoute"));
	RootComponent = PatrolRoute;
	PatrolBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("PatrolBoxCollision"));
	PatrolBoxCollision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATeam_AIPatrolRoute::BeginPlay()
{
	Super::BeginPlay();
	PatrolRoute->SetVisibility(false);
}

void ATeam_AIPatrolRoute::OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("OutPatrolRoute!")));
	ATeam_AICharacterBase* AICharacter = Cast<ATeam_AICharacterBase>(OtherActor);
	if (!AICharacter)
		return;
	ATeam_AIController* AIController = Cast<ATeam_AIController>(AICharacter->GetController());
	if (!AIController)
		return;
	UBehaviorTreeComponent* BehaviorTree = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent());
	if (!BehaviorTree)
		return;
	UBlackboardComponent* BlackBoard = AIController->GetBlackboardComponent();
	if (!BlackBoard)
		return;
	BehaviorTree->PauseLogic(TEXT("Collision OutOverlap"));
	BehaviorTree->ResumeLogic(TEXT("Resume Overlap"));
	AIController->SetState(ECharacterState::PATROL);
	BlackBoard->SetValueAsVector(ATeam_AIController::PatrolPosKey, PatrolRoute->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World));
	BlackBoard->SetValueAsObject(ATeam_AIController::TargetKey, nullptr);
	BlackBoard->SetValueAsBool(ATeam_AIController::TargetInRangeKey, false);
	//BlackBoard->SetValueAsBool(ATeam_AIController::AttackAvailableKey, true);
}

// Called every frame
void ATeam_AIPatrolRoute::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATeam_AIPatrolRoute::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PatrolBoxCollision->OnComponentEndOverlap.AddDynamic(this, &ATeam_AIPatrolRoute::OnCollisionEndOverlap);
}

int32 ATeam_AIPatrolRoute::GetSizeOfPatrolRoute() const
{
	return PatrolRoute->GetNumberOfSplinePoints();
}

FVector ATeam_AIPatrolRoute::GetLocationAtIndex(int32 index) const
{
	return PatrolRoute->GetLocationAtSplinePoint(index, ESplineCoordinateSpace::World);
}

bool ATeam_AIPatrolRoute::CheckSpawnPossible() const
{
	for (const auto& spawnPoint : SpawnPoints)
	{
		if (spawnPoint->GetSpawnActor())
			return false;
	}
	return true;
}

const TArray<class ATeam_AISpawnPointPatrol*>& ATeam_AIPatrolRoute::GetSpawnPoints() const
{
	return SpawnPoints;
}

ATeam_AISpawnPointPatrol* ATeam_AIPatrolRoute::GetSpawnPoint(int32 idx)
{
	return SpawnPoints[idx];
}