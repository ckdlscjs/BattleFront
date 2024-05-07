// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Team_AICharacterBase.h"
#include "Team_AICharacter_Recv.generated.h"

/**
 * 
 */
DECLARE_DELEGATE(PacketAttackDelegate);
DECLARE_DELEGATE(PacketDeadDelegate);
DECLARE_DELEGATE_ThreeParams(PacketMoveDelegate, FVector, FRotator, float);

UCLASS()
class AI_MAP_API ATeam_AICharacter_Recv : public ATeam_AICharacterBase
{
	GENERATED_BODY()
	
public:
	virtual void PostInitializeComponents() override;
protected:
	virtual void RecvTest(FString str) override;
private:

public:
protected:
	PacketAttackDelegate AIAttack;
	PacketAttackDelegate AIDead;
	PacketMoveDelegate AIMove;
private:
	
};
