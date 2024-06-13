// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class AI_MAP_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetPercentage(float MaxHp, float CurrHp);
	void SetGuardPercent(float MaxGuard, float CurrGurad);
	void SetColorHpBar();
protected:
	virtual void NativeConstruct() override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite , meta = (BindWidget))
		class UProgressBar* HealthBar;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* GuardBar;
	UPROPERTY()
		class UCanvasPanelSlot* PanelSlot;
};
