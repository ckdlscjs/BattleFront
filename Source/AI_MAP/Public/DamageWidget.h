// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageWidget.generated.h"

/**
 * 
 */
UCLASS()
class AI_MAP_API UDamageWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void PlayAnim();
	void SetDamageText(float DamageAmount);
	void InitWidget();
	bool GetIsAnimEnd();
	UFUNCTION()
		void AnimationFinished();
public:
	UPROPERTY(BlueprintReadOnly, Category = "BindAnimation", meta = (BindWidgetAnim), Transient)
		class UWidgetAnimation* TextAnimationRight;	
	UPROPERTY(BlueprintReadOnly, Category = "BindAnimation", meta = (BindWidgetAnim), Transient)
		class UWidgetAnimation* TextAnimationLeft;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Damage;

	UPROPERTY()
		bool bIsAnimEnd;
	FWidgetAnimationDynamicEvent EndDelegate;
};
