// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillWidget.generated.h"

/**
 * 
 */
UCLASS()
class AI_MAP_API USkillWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateSkillImage(class UTexture2D* Texture, int32 index);
	void UpdateSkillLevel(int32 Index, int32 Level);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UHorizontalBox* SkillImageSet;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* Q_SkillIamge;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* W_SkillIamge;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* E_SkillIamge;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* R_SkillIamge;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UImage* T_SkillIamge;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UHorizontalBox* SkillLevelSet;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USkillLevelWidget* Q_SkillWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USkillLevelWidget* W_SkillWidget; 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USkillLevelWidget* E_SkillWidget; 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USkillLevelWidget* R_SkillWidget; 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USkillLevelWidget* T_SkillWidget;

	UPROPERTY()
		TMap<class UImage*, class USkillLevelWidget*> SkillArr;
	UPROPERTY()
		TMap<int32, class UImage*> SkillImageArr;

protected:
	virtual void NativeOnInitialized()override;
};
