// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChooseHostWidget.generated.h"

/**
 * 
 */
UCLASS()
class AI_MAP_API UChooseHostWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
public:
	UFUNCTION()
		void OkButtonClick();
	UFUNCTION()
		void HostCheckBoxClicked(bool bIsCheck);
	UFUNCTION()
		void ClientCheckBoxClicked(bool bIsCheck);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UCheckBox* HostCheck;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UCheckBox* ClientCheck;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* OK_Button;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsHost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<UUserWidget> HostWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<UUserWidget> ClientWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UHostScreen* HostScreen;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UClientScreen* ClientScreen;

public:
	class UHostScreen* GetHostScreen();
	class UClientScreen* GetClientScreen();
	class UNetworkManager* GetNetworkManager() const;
};
