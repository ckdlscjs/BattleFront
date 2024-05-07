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
  
    if (AICharacter->GetDistanceTo(Target) <= AICharacter->GetAttackRange())
    {
        //UE_LOG(LogTemp, Error, TEXT("IsInRange"));
        Blackboard->SetValueAsBool(ATeam_AIController::TargetInRangeKey, true);
    }
    else
    {
        //UE_LOG(LogTemp, Error, TEXT("NotInRange"));
        if(!AICharacter->IsAttacking())
            Blackboard->SetValueAsBool(ATeam_AIController::TargetInRangeKey, false);
    }
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