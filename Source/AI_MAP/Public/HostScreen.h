// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HostScreen.generated.h"

/**
 * 
 */
UCLASS()
class AI_MAP_API UHostScreen : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
public:
	UFUNCTION()
		void GameStartButtonClick();
	void SetPlayerCount(int32 PlayerCount);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))

		class UTextBlock* PlayerNum; //player count
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* GameStartButton;
};
