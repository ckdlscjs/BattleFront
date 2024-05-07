// Fill out your copyright notice in the Description page of Project Settings.


#include "EQC_WallCheck.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Team_AIController.h"
#include "Team_AICharacterBase.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
void UEQC_WallCheck::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	ATeam_AICharacterBase* Character = Cast<ATeam_AICharacterBase>(QueryInstance.Owner.Get());
	if (!Character)
		return;
	ATeam_AIController* AIController = Cast<ATeam_AIController>(Character->GetController());
	if (!AIController)
		return;
	UBlackboardComponent* BlackBoard = AIController->GetBlackboardComponent();
	if (!BlackBoard)
		return;
	AActor* Target = Cast<AActor>(BlackBoard->GetValueAsObject(ATeam_AIController::TargetKey));
	if (!Target)
		return;
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, Target);
}
