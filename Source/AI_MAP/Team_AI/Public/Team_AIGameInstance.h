// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI_MAP.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Team_AIGameInstance.generated.h"

/**
 * 
 */

USTRUCT()
struct FTeam_CharacterData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Armor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Ability;
	UPROPERTY(EditAnywhere)
		float Percentage;

	UPROPERTY(EditAnywhere)
		float DropExp;

	UPROPERTY(EditAnywhere)
		float NextExp;
};


UCLASS()
class AI_MAP_API UTeam_AIGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UTeam_AIGameInstance();
	virtual void Init() override;
	FTeam_CharacterData* GetCharacterRowData(int32 Level);
	UFUNCTION(BlueprintCallable)
	int32 GetSizeCharacterDataTable() const;
	
private:
	UPROPERTY()
		UDataTable* CharcterDataTable;
		
};
