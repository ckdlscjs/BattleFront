// Fill out your copyright notice in the Description page of Project Settings.

#include "DamageWidget.h"
#include "Components/TextBlock.h"

void UDamageWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UDamageWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry,InDeltaTime);
}

void UDamageWidget::PlayAnim()
{
	int32 RandNum = FMath::FRandRange(0, 10);
	if (RandNum % 2 == 0)
	{
		PlayAnimation(TextAnimationRight);
	}
	else
	{
		PlayAnimation(TextAnimationLeft);
	}
}

void UDamageWidget::SetDamageText(float DamageAmount)
{
	int32 DM = DamageAmount;
	FText DT = FText::FromString(FString::FromInt(DM));

	Damage->SetText(DT);
}

void UDamageWidget::InitWidget()
{
	Super::NativeConstruct();

	bIsAnimEnd = false;
	EndDelegate.BindDynamic(this, &UDamageWidget::AnimationFinished);
	BindToAnimationFinished(TextAnimationRight, EndDelegate);
	BindToAnimationFinished(TextAnimationLeft, EndDelegate);
}

bool UDamageWidget::GetIsAnimEnd()
{
	return bIsAnimEnd;
}

void UDamageWidget::AnimationFinished()
{
	bIsAnimEnd = true;

}
