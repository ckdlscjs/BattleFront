// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AttackAvailable.h"
#include "Team_AICharacterBase.h"
#include "Team_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
UBTTask_AttackAvailable::UBTTask_AttackAvailable()
{

}

EBTNodeResult::Type UBTTask_AttackAvailable::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
    //Blackboard->SetValueAsBool(ATeam_AIController::AttackAvailableKey, true);
	return EBTNodeResult::Succeeded;
}
