// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI_MAP.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EQC_WallCheck.generated.h"

/**
 * 
 */
UCLASS()
class AI_MAP_API UEQC_WallCheck : public UEnvQueryContext
{
	GENERATED_BODY()

	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const;
};
