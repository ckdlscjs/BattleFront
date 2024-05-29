// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_TargetInRange.h"
#include "Team_AIController.h"
#include "Team_AICharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UBTService_TargetInRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
    AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ATeam_AIController::TargetKey));
    if (!Target)
        return;
    if (AICharacter->GetDistanceTo(Target) > AICharacter->GetDetectRadius())
    {
        Blackboard->SetValueAsObject(ATeam_AIController::TargetKey, nullptr);
        return;
    }
    if (AICharacter->GetDistanceTo(Target) <= AICharacter->GetAttackRange())
    {
        Blackboard->SetValueAsBool(ATeam_AIController::TargetInRangeKey, true);
    }
    else
    {
        if(!AICharacter->IsAttacking())
            Blackboard->SetValueAsBool(ATeam_AIController::TargetInRangeKey, false);
    } 
}