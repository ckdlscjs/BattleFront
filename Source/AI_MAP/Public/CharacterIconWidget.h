// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterIconWidget.generated.h"

/**
 * 
 */
UCLASS()
class AI_MAP_API UCharacterIconWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	class USkillWidget* GetSkillWidget();
	void UpdateSkillImage(class UTexture2D* Texture,int32 index);
	void UpdateSkillLevel(int32 index, int32 level);
	void UpdateHpBar(int32 MaxHp, int32 CurrHp);
	void UpdateExpBar(int32 MaxExp, int32 CurrExp);
protected:
	virtual void NativeOnInitialized() override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* CharIconImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UTextBlock* CharLevel;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UProgressBar* ExpBar;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USkillWidget* Skill;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UStatusBar* StatBar;
};
