// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AIMoveTo.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Team_AIController.h"
#include "Team_AICharacterBase.h"
#include "Team_AIPatrolRoute.h"


UBTTask_AIMoveTo::UBTTask_AIMoveTo()
{
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_AIMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ATeam_AIController* AIController = Cast<ATeam_AIController>(OwnerComp.GetAIOwner());
    if (!AIController)
        return EBTNodeResult::Failed;
    ATeam_AICharacterBase* AICharacter = Cast<ATeam_AICharacterBase>(AIController->GetPawn());
    if (!AICharacter)
        return EBTNodeResult::Failed;
    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard)
        return EBTNodeResult::Failed;

    OwnerComp.GetAIOwner()->MoveToLocation(Blackboard->GetValueAsVector(ATeam_AIController::PatrolPosKey), AcceptanceRadius);
    return EBTNodeResult::InProgress;
}

void UBTTask_AIMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    ATeam_AIController* AIController = Cast<ATeam_AIController>(OwnerComp.GetAIOwner());
    if (!AIController)
        return;
    ATeam_AICharacterBase* AICharacter = Cast<ATeam_AICharacterBase>(AIController->GetPawn());
    if (!AICharacter)
        return;
    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard)
        return;

    AICharacter->SendTest("MoveTo");
}

