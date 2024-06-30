// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IPWidget.generated.h"

/**
 * 
 */
UCLASS()
class AI_MAP_API UIPWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
public:
	UFUNCTION()
		void InputButtonClick();
	UFUNCTION()
		void HostCheckBoxClicked(bool bIsCheck);
	UFUNCTION()
		void ClientCheckBoxClicked(bool bIsCheck);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UEditableTextBox* IP_Text;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UCheckBox* HostCheck;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UCheckBox* ClientCheck;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* InputButton;
	UPROPERTY()
		bool bIsHost;

public:
	class UNetworkManager* GetNetworkManager() const;
};
