// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPos.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Team_AIController.h"
#include "Team_AICharacterBase.h"
UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

    ATeam_AICharacterBase* Character = Cast<ATeam_AICharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
    if (Character == nullptr)
        return EBTNodeResult::Failed;

    auto NaviSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
    if(NaviSystem == nullptr)
        return EBTNodeResult::Failed;
    
    FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(ATeam_AIController::SpawnPosKey);

    FNavLocation NextLocation;
    if (NaviSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, Character->GetPatrolRadius(), NextLocation))
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(ATeam_AIController::PatrolPosKey, NextLocation.Location);
        /*FVector Rotate = NextLocation.Location - Character->GetActorLocation();
        Rotate.Normalize();
        Character->SetActorRotation(Rotate.Rotation());*/
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
