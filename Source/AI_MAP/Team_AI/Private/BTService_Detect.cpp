// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "Team_AIController.h"
#include "Team_AICharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameCharacter.h"
void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	ATeam_AIController* AIController = Cast<ATeam_AIController>(OwnerComp.GetAIOwner());
	if (!AIController)
		return;
	ATeam_AICharacterBase* AICharacter = Cast<ATeam_AICharacterBase>(AIController->GetPawn());
	if (!AICharacter)
		return;
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
		return;

	AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(ATeam_AIController::TargetKey));
	if (IsValid(Target))
	{
		if (Cast<AGameCharacter>(Target)->IsDead())
		{
			Blackboard->SetValueAsObject(ATeam_AIController::TargetKey, nullptr);
			AIController->SetState(ECharacterState::IDLE);
			return;
		}
		AIController->SetState(ECharacterState::ATTACK);
	}
	else if(AICharacter->GetPatrolRoute())
	{
		AIController->SetState(ECharacterState::PATROL);
	}
	else
	{
		AIController->SetState(ECharacterState::INVESTIGATING);
	}
}
