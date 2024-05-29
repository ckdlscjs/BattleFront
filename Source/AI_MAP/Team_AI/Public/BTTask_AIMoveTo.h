// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI_MAP.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AIMoveTo.generated.h"


/**
 * 
 */
UCLASS()
class AI_MAP_API UBTTask_AIMoveTo : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_AIMoveTo();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
protected:
	UPROPERTY(EditAnywhere)
	float AcceptanceRadius = 50.0f;

private:

};
