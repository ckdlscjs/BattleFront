#include "BTTask_AttackWait.h"
#include "Team_AICharacterBase.h"
#include "Team_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
EBTNodeResult::Type UBTTask_AttackWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    bNotifyTick = true;
    CurrentDelay = 0.0f;
    return EBTNodeResult::InProgress;
}

void UBTTask_AttackWait::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
    CurrentDelay += DeltaSeconds;
    FinishLatentTask(OwnerComp, CurrentDelay > Blackboard->GetValueAsFloat(ATeam_AIController::AttackDelayKey) ? EBTNodeResult::Succeeded : EBTNodeResult::InProgress);
}
