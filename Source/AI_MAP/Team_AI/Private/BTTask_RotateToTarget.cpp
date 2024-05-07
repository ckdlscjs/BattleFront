// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RotateToTarget.h"
#include "Team_AIController.h"
#include "Team_AICharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_RotateToTarget::UBTTask_RotateToTarget()
{
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_RotateToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
    AICharacter->SendTest("Rotate");
    if (AIController->GetState() == ECharacterState::ATTACK)
    {
        AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ATeam_AIController::TargetKey));
        if (!Target)
            return EBTNodeResult::Failed;
        Blackboard->SetValueAsVector(ATeam_AIController::PatrolPosKey, Target->GetActorLocation());
    }
    OwnerComp.GetAIOwner()->StopMovement();
    return EBTNodeResult::InProgress;
}

void UBTTask_RotateToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

    // 방향 벡터를 사용하여 회전 값 계산
    FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(AICharacter->GetActorLocation(), Blackboard->GetValueAsVector(ATeam_AIController::PatrolPosKey));

    // Yaw 값만 사용하여 Target 회전 값에서 Yaw 회전 값을 추출
    FRotator TargetYawRotation = FRotator(0, TargetRotation.Yaw, 0);

    // 현재 캐릭터의 회전 값을 가져옴
    FRotator CurrentRotation = AICharacter->GetActorRotation();

    // 보간된 회전 값 계산
    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetYawRotation, DeltaSeconds, 10.0f);

    // 캐릭터의 회전 값을 설정
    AICharacter->SetActorRotation(NewRotation);
    
    //Server
    AICharacter->SendTest("RotateTo");

    if (UKismetMathLibrary::Abs(CurrentRotation.Yaw - TargetYawRotation.Yaw) < 0.1f)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
