// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_Hideable.h"
#include "Team_AIController.h"
#include "Team_AICharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UBTDecorator_Hideable::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, AICharacter);
	GetWorld()->OverlapMultiByProfile(OverlapResults, AICharacter->GetActorLocation(), FQuat::Identity, FName("WallCheck"), FCollisionShape::MakeBox(FVector(1000.0f, 1000.0f, 10.0f)), CollisionQueryParam);
    if(OverlapResults.Num())
    {
        //DrawDebugBox(GetWorld(), AICharacter->GetActorLocation(), FVector(1000.0f, 1000.0f, 10.0f), FColor::Red);
        return true;
    }
    return false;
}
