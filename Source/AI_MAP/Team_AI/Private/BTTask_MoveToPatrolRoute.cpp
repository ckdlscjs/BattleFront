// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToPatrolRoute.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Team_AIController.h"
#include "Team_AICharacterBase.h"
#include "Team_AIPatrolRoute.h"

UBTTask_MoveToPatrolRoute::UBTTask_MoveToPatrolRoute()
{
    //bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MoveToPatrolRoute::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

    ATeam_AIPatrolRoute* PatrolRoute = AICharacter->GetPatrolRoute();
    if(PatrolRoute == nullptr)
        return EBTNodeResult::Failed;

    if (!IsValid(PatrolRoute))
        return EBTNodeResult::Failed;
    Blackboard->SetValueAsVector(ATeam_AIController::PatrolPosKey, PatrolRoute->GetLocationAtIndex(AICharacter->GetCurrentPatrolIndex()));
    //OwnerComp.GetAIOwner()->MoveToLocation(,50.0f);
    return EBTNodeResult::Succeeded;
}
