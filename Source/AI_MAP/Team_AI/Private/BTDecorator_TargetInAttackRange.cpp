// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_TargetInAttackRange.h"
#include "Team_AIController.h"
#include "Team_AICharacter_Boss.h"
#include "BehaviorTree/BlackboardComponent.h"
/*
* Boss TargetInRange
*/
UBTDecorator_TargetInAttackRange::UBTDecorator_TargetInAttackRange()
{
    bNotifyTick = false;
    NodeName = TEXT("CanAttack");
}

bool UBTDecorator_TargetInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
   
    ATeam_AIController* AIController = Cast<ATeam_AIController>(OwnerComp.GetAIOwner());
    if (!AIController)
        return false;
    ATeam_AICharacter_Boss* AICharacter = Cast<ATeam_AICharacter_Boss>(AIController->GetPawn());
    if (!AICharacter)
        return false;
    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard)
        return false;
    if (AICharacter->RecognizePlayers())
    {
        AIController->SetState(ECharacterState::ATTACK);
        return true;
    }
    AIController->SetState(ECharacterState::IDLE);
    return false;

    /*if (AICharacter->RecognizePlayers())
    {
        UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("TruePlayers")));
        return true;
    }
    else
    {
        UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("FalsePlayers")));
        return false;
    }*/
    /*
    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams CollisionQueryParam(NAME_None, false, AICharacter);
    GetWorld()->OverlapMultiByProfile(OverlapResults, AICharacter->GetActorLocation(), FQuat::Identity, FName("PlayerCheck"), FCollisionShape::MakeSphere(AICharacter->GetDetectRadius()), CollisionQueryParam);
    if (OverlapResults.Num())
    {
        for (const auto& iter : OverlapResults)
        {
            UKismetSystemLibrary::PrintString(GetWorld(), FString(iter.GetActor()->GetName()), true, false);
            if (iter.GetActor()->ActorHasTag(AICharacter->GetTargetTag()))
            {
                AIController->SetState(ECharacterState::ATTACK);
                OwnerComp.GetBlackboardComponent()->SetValueAsObject(ATeam_AIController::TargetKey, iter.GetActor()); //change to attackstate
                //UKismetSystemLibrary::PrintString(GetWorld(), FString(iter.GetActor()->GetActorNameOrLabel()), true, false);
                DrawDebugLine(GetWorld(), AICharacter->GetActorLocation(), iter.GetActor()->GetActorLocation(), FColor::Red, false, 0.3f);
                DrawDebugSphere(GetWorld(), AICharacter->GetActorLocation(), AICharacter->GetDetectRadius(), 16, FColor::Blue, false, 1.0f);
            }
        }
    }
    else
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsObject(ATeam_AIController::TargetKey, nullptr);
    }
    */
}

