// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class AI_MAP_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//void CreateSubWidget();
	void ShowChooseSkill();
	void HideChooseSkill();
	void SetChooseSkillImage(TArray<class UTexture2D*>& RadomTextureArray);
	void UpdateSkillLevel(int32 Index, int32 Level);
	void UpdateSkillImage(class UTexture2D* Texture,int32 Index);
	void UpdateHpBar(int32 MaxHP, int32 CurrHp);
	void UpdateExpBar(float MaxExp, float CurrExp);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UCharacterIconWidget* CharacterWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USkillChooseWidget* ChooseSkillWidget;
	UPROPERTY()
		class UCanvasPanelSlot* PanelSlot;
protected:
	virtual void NativeConstruct() override;
};