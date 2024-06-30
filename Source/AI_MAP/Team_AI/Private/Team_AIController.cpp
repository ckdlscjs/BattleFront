// Fill out your copyright notice in the Description page of Project Settings.


#include "Team_AIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Team_AICharacterBase.h"
#include "BTTask_MoveToPatrolRoute.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "GameCharacter.h"

const FName ATeam_AIController::SpawnPosKey(TEXT("SpawnPos"));
const FName ATeam_AIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ATeam_AIController::TargetKey(TEXT("Target"));
const FName ATeam_AIController::TargetInRangeKey(TEXT("TargetInRange"));
const FName ATeam_AIController::StateKey(TEXT("State"));
const FName ATeam_AIController::AttackDelayKey(TEXT("AttackDelay"));
//const FName ATeam_AIController::AttackAvailableKey(TEXT("AttackAvailable"));


ATeam_AIController::ATeam_AIController()
{
	/*
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTree(TEXT("/Game/AI/BT_Team_Character.BT_Team_Character"));
	if (BehaviorTree.Succeeded())
	{
		BT_Asset = BehaviorTree.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BlackboardData(TEXT("/Game/AI/BB_Team_Character.BB_Team_Character"));
	if (BlackboardData.Succeeded())
	{
		BB_Asset = BlackboardData.Object;
	}
	*/
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SenseSight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISenseSight"));
	SenseDamage = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("AISenseDamage"));
	Duration_DetectRadius = 1.0f;

	SenseSight->SightRadius = 500.0f;
	SenseSight->LoseSightRadius = 1000.0f;
	SenseSight->PeripheralVisionAngleDegrees = 60.0f; // �ֺ� �þ߰�
	SenseSight->DetectionByAffiliation.bDetectEnemies = true; // �ҼӺ� Ž�� ��
	SenseSight->DetectionByAffiliation.bDetectFriendlies = true; // �ҼӺ� Ž�� ��
	SenseSight->DetectionByAffiliation.bDetectNeutrals = true; // �ҼӺ� Ž�� �߸�
	SenseSight->SetMaxAge(1.0f);
	PerceptionComponent->ConfigureSense(*SenseSight);

	SenseDamage->SetMaxAge(1.0f);
	PerceptionComponent->ConfigureSense(*SenseDamage);
	PerceptionComponent->SetDominantSense(SenseSight->GetSenseImplementation());
	PerceptionComponent->Activate();
}

void ATeam_AIController::OnAIMoveComplete(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	ATeam_AICharacterBase* AICharacter = Cast<ATeam_AICharacterBase>(GetPawn());
	if (!AICharacter)
		return;
	// �̵� �Ϸ� �� Behavior Tree�� ���¸� ������Ʈ
	UBehaviorTreeComponent* BehaviorTree = Cast<UBehaviorTreeComponent>(GetBrainComponent());
	if (!BehaviorTree)
		return;
	BehaviorTree->OnTaskFinished(Cast<UBTTaskNode>(BehaviorTree->GetActiveNode()), EBTNodeResult::Succeeded);
	AICharacter->SendTest("MoveEnd");
	if(AICharacter->GetCurrentState() == ECharacterState::PATROL)
		AICharacter->IncrementPatrolIndex();
}

void ATeam_AIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ReceiveMoveCompleted.AddDynamic(this, &ATeam_AIController::OnAIMoveComplete);
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ATeam_AIController::OnSensed);
}

void ATeam_AIController::SetState(ECharacterState State)
{
	if (!Blackboard)
		return;
	Blackboard->SetValueAsEnum(StateKey, (uint8)State);

	ATeam_AICharacterBase* AICharacter = Cast<ATeam_AICharacterBase>(GetPawn());
	if (!AICharacter)
		return;
	AICharacter->SetCurrentState(State);
}

ECharacterState ATeam_AIController::GetState() const
{
	if (!Blackboard)
		return ECharacterState::IDLE;
	return (ECharacterState)Blackboard->GetValueAsEnum(StateKey);
}

void ATeam_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATeam_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ATeam_AICharacterBase* AICharacter = Cast<ATeam_AICharacterBase>(InPawn);
	
	if (UseBlackboard(BB_Asset, (UBlackboardComponent*)Blackboard))
	{
		Blackboard->SetValueAsVector(SpawnPosKey, AICharacter->GetActorLocation());
		Blackboard->SetValueAsFloat(AttackDelayKey, AICharacter->GetAttackDelay());
		//Blackboard->SetValueAsBool(AttackAvailableKey, true);

		if (!RunBehaviorTree(BT_Asset))
		{
			UE_LOG(LogTemp, Warning, TEXT("NotRunBt"));
		}
	}
	GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &ATeam_AIController::OnTargetDetect, Duration_DetectRadius, true);
}

void ATeam_AIController::OnUnPossess()
{
	Super::OnUnPossess();
	//GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}

void ATeam_AIController::OnTargetDetect()
{
	if ((ECharacterState)Blackboard->GetValueAsEnum(ATeam_AIController::StateKey) != ECharacterState::ATTACK)
		return;
	ATeam_AICharacterBase* AICharacter = Cast<ATeam_AICharacterBase>(GetPawn());
	if (!AICharacter)
		return;
	AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(ATeam_AIController::TargetKey));

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, AICharacter);
	GetWorld()->OverlapMultiByProfile(OverlapResults, AICharacter->GetActorLocation(), FQuat::Identity, FName("PlayerCheck"), FCollisionShape::MakeSphere(AICharacter->GetDetectRadius()), CollisionQueryParam);
	if (OverlapResults.Num())
	{
		for (const auto& iter : OverlapResults)
		{
			if (iter.GetActor()->ActorHasTag(AICharacter->GetTargetTag()))
			{
				if (iter.GetActor() != Target)
					continue;
				if (AICharacter->GetDistanceTo(iter.GetActor()) >= SenseSight->SightRadius)
				{
					//UKismetSystemLibrary::PrintString(GetWorld(), FString(iter.GetActor()->GetActorNameOrLabel()), true, false);
					Blackboard->SetValueAsObject(ATeam_AIController::TargetKey, nullptr);
					Blackboard->SetValueAsEnum(ATeam_AIController::StateKey, (uint8)ECharacterState::IDLE);
					//DrawDebugLine(GetWorld(), AICharacter->GetActorLocation(), iter.GetActor()->GetActorLocation(), FColor::Red, false, 0.3f);
					//DrawDebugSphere(GetWorld(), AICharacter->GetActorLocation(), AICharacter->GetDetectRadius(), 16, FColor::Blue, false, 1.0f);
				}
			}
		}
	}
	else
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), FString(iter.GetActor()->GetActorNameOrLabel()), true, false);
		Blackboard->SetValueAsObject(ATeam_AIController::TargetKey, nullptr);
		Blackboard->SetValueAsEnum(ATeam_AIController::StateKey, (uint8)ECharacterState::IDLE);
		//DrawDebugSphere(GetWorld(), AICharacter->GetActorLocation(), AICharacter->GetDetectRadius(), 16, FColor::Blue, false, 1.0f);
	}
}

void ATeam_AIController::OnSensed(const TArray<AActor*>& UpdatedActors)
{
	ATeam_AICharacterBase* AICharacter = Cast<ATeam_AICharacterBase>(GetPawn());
	if (!AICharacter)
		return;
	if (!Blackboard)
		return;
	for (const auto& actor : UpdatedActors)
	{
		if (!actor->ActorHasTag(AICharacter->GetTargetTag()))
			continue;
		FActorPerceptionBlueprintInfo PerceptionInfo;
		PerceptionComponent->GetActorsPerception(actor, PerceptionInfo);
		for (const auto& sense : PerceptionInfo.LastSensedStimuli)
		{
			if (UAISense_Sight::StaticClass() == UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), sense))
			{
				auto Target = Cast<AActor>(Blackboard->GetValueAsObject(TargetKey));
				Target = !Target ? actor : (AICharacter->GetDistanceTo(actor) < AICharacter->GetDistanceTo(Target)) ? actor : Target;
				if (Cast<AGameCharacter>(Target)->IsDead())
				{
					Blackboard->SetValueAsObject(TargetKey, nullptr);
					return;
				}
				Blackboard->SetValueAsObject(TargetKey, Target);
			}

			if (UAISense_Damage::StaticClass() == UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), sense))
			{
				if (Cast<AGameCharacter>(actor)->IsDead())
				{
					Blackboard->SetValueAsObject(TargetKey, nullptr);
					return;
				}
				Blackboard->SetValueAsObject(TargetKey, actor);
			}
		}
	}
}
