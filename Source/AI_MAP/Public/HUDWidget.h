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
	void UpdateGuardPoint(int32 Cur, int32 Max);
	void SetCharacterLevel(int32 Level);
	void SetMageticeFieldTimer(float Timer);

	void ToggleWorldMap();
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UCharacterIconWidget* CharacterWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USkillChooseWidget* ChooseSkillWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* CurrentGuardText;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* MaxGuardText;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TimerText;
	UPROPERTY()
		class UCanvasPanelSlot* PanelSlot;
	UPROPERTY()
		const class ATeam_AIMagneticField* MagneticField;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UWorldMapWidget* WorldMapWidget;
	bool WorldMapVisible;
	float MyTimer;
	
protected:
	virtual void NativeConstruct() override;
};
