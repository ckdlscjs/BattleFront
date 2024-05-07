// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "AI_MAP.h"
#include "BehaviorTree/BTService.h"
#include "BTService_TargetInRange.generated.h"

/**
 * 
 */
UCLASS()
class AI_MAP_API UBTService_TargetInRange : public UBTService
{
	GENERATED_BODY()
public:

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
};
