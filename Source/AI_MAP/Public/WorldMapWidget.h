// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI_MAP.h"
#include "Blueprint/UserWidget.h"
#include "WorldMapWidget.generated.h"

/**
 * 
 */
UCLASS()
class AI_MAP_API UWorldMapWidget : public UUserWidget
{
	GENERATED_BODY()
public:
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
private:

public:
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* PlayerIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* PlayerCharacter;
private:
	
};
