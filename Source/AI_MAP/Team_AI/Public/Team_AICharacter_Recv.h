// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Team_AICharacterBase.h"
#include "Team_AICharacter_Recv.generated.h"

/**
 * 
 */
//DECLARE_DELEGATE(PacketAttackDelegate);
//DECLARE_DELEGATE(PacketDeadDelegate);
//DECLARE_DELEGATE_ThreeParams(PacketMoveDelegate, FVector, FRotator, float);

class UParticleSystemComponent;
UCLASS()
class AI_MAP_API ATeam_AICharacter_Recv : public ATeam_AICharacterBase
{
	GENERATED_BODY()
	
public:
	ATeam_AICharacter_Recv();
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	bool RecvMoveTo(FVector loc, FRotator rot, float speed);
	bool RecvAttack(int32 idx = 0);
	//bool RecvAttackParticle(); //NotUse
	bool RecvRotate(FRotator rot);
	bool RecvDead();
protected:
	/*virtual void RecvTest(FString str) override;*/

private:

public:
protected:
	/*PacketAttackDelegate AIAttack;
	PacketAttackDelegate AIDead;
	PacketMoveDelegate AIMove;*/
private:
	
};
