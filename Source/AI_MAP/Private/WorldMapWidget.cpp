// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldMapWidget.h"
#include "CharacterController.h"

void UWorldMapWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerIcon = Cast<UUserWidget>(GetWidgetFromName(TEXT("Icon")));
	auto playerController = Cast<ACharacterController>(UGameplayStatics::GetPlayerController(this, 0));
	if (!playerController)
		return;
	PlayerCharacter = playerController->GetGameCharacter();
}

void UWorldMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!PlayerCharacter)
		return;
	FVector Loc = PlayerCharacter->GetActorLocation();
	Loc.X = (Loc.X / 25000.0f * 1024.0f * -1.0f) + 512.0f;
	Loc.Y = (Loc.Y / 25000.0f * 1024.0f ) + 512.0f;
	PlayerIcon->SetRenderTranslation({ Loc.Y, Loc.X });
}
