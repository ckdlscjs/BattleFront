// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI_Map.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_RandBasedLocation.generated.h"

/**
 * 
 */
UCLASS()
class AI_MAP_API UBTDecorator_RandBasedLocation : public UBTDecorator
{
	GENERATED_BODY()
public:
	//UBTDecorator_RandBasedLocation();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
private:
};
