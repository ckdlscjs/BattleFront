// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_RandBasedLocation.h"
#include "Team_AIController.h"
#include "Team_AICharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
bool UBTDecorator_RandBasedLocation::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    ATeam_AIController* AIController = Cast<ATeam_AIController>(OwnerComp.GetAIOwner());
    if (!AIController)
        return false;
    ATeam_AICharacterBase* AICharacter = Cast<ATeam_AICharacterBase>(AIController->GetPawn());
    if (!AICharacter)
        return false;
    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard)
        return false;
    auto Target = Cast<AActor>(Blackboard->GetValueAsObject(ATeam_AIController::TargetKey));
    if (!Target)
        return false;
    return (uint32)(Target->GetActorLocation().X + Target->GetActorLocation().Y) % 2 == 0 ?  true : false;
}
