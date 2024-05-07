// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillLevelWidget.generated.h"

/**
 * 
 */
UCLASS()
class AI_MAP_API USkillLevelWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetSkillLevel(int32 level);
protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UStackBox* LevelBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UCheckBox* Level1_Check;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UCheckBox* Level2_Check; 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UCheckBox* Level3_Check;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UCheckBox* Level4_Check;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UCheckBox* Level5_Check;
	UPROPERTY()
		TArray<class UCheckBox*> LevelArray;
};
