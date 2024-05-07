// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI_MAP.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AttackAvailable.generated.h"

/**
 * 
 */
UCLASS()
class AI_MAP_API UBTTask_AttackAvailable : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_AttackAvailable();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
