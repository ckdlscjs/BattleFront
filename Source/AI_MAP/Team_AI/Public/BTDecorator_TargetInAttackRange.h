// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI_MAP.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_TargetInAttackRange.generated.h"

/**
* Change Decorator to Service
 * 
 */
UCLASS()
class AI_MAP_API UBTDecorator_TargetInAttackRange : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_TargetInAttackRange();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
private:
};
