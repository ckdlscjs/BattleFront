// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClientScreen.generated.h"

/**
 * 
 */
UCLASS()
class AI_MAP_API UClientScreen : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
public:
	void SetPlayerCount(int32 PlayerCount);
	UFUNCTION()
		void ConnectButtonClick();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))

		class UTextBlock* PlayerNum; //player count
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UEditableTextBox* Ip_Text;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* ConnectButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))

		class UTextBlock* SuccessText;
};
