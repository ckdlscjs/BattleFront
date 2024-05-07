// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillChooseWidget.generated.h"

/**
 * 
 */
UCLASS()
class AI_MAP_API USkillChooseWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetSelectSkillImage(TArray<class UTexture2D*>& TextureArray);
protected:
	virtual void NativeOnInitialized() override;	

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* FirSkill;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* SecSkill;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* ThirSkill;
	UPROPERTY()
		TArray<class UImage*> ImageArray;
private:
	FLinearColor ActivateColor;
	FLinearColor UnActivateColor;
};
