// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack_Boss.h"
#include "Team_AICharacter_Boss.h"
#include "Team_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
UBTTask_Attack_Boss::UBTTask_Attack_Boss()
{
}

EBTNodeResult::Type UBTTask_Attack_Boss::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ATeam_AIController* AIController = Cast<ATeam_AIController>(OwnerComp.GetAIOwner());
    if (!AIController)
        return EBTNodeResult::Failed;
    ATeam_AICharacter_Boss* AICharacter = Cast<ATeam_AICharacter_Boss>(AIController->GetPawn());
    if (!AICharacter)
        return EBTNodeResult::Failed;
    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard)
        return EBTNodeResult::Failed;
    /*if (!Blackboard->GetValueAsBool(ATeam_AIController::AttackAvailableKey))
        return EBTNodeResult::Failed;*/
    int AttackIdx = AICharacter->GetRandomAttackIdx();
    AICharacter->BehaviorAttack(AttackIdx);
    AICharacter->SendTest("Attack", AttackIdx);
    //Blackboard->SetValueAsBool(ATeam_AIController::AttackAvailableKey, false);
    return EBTNodeResult::InProgress;
}
