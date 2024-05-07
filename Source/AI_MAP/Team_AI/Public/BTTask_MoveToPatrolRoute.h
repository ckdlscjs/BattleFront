// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI_MAP.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToPatrolRoute.generated.h"

/**
 * 
 */
UCLASS()
class AI_MAP_API UBTTask_MoveToPatrolRoute : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_MoveToPatrolRoute();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	//virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
