// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageTextActor.h"
#include "Components/WidgetComponent.h"
#include "DamageWidget.h"
// Sets default values
ADamageTextActor::ADamageTextActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DamageWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Damage Text"));
	DamageWidgetComponent->SetupAttachment(RootComponent);
}
// Called when the game starts or when spawned
void ADamageTextActor::BeginPlay()
{
	Super::BeginPlay();
	if (DamageWidgetClass != nullptr)
	{
		DamageWidgetComponent->SetWidgetClass(DamageWidgetClass);
	}
}

// Called every frame
void ADamageTextActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (DamageWidget != nullptr)
	{
		if (DamageWidget->GetIsAnimEnd())
		{
			Destroy();
		}
	}
}

void ADamageTextActor::SpawnDamageText(float Damage)
{
	DamageWidget = Cast<UDamageWidget>(DamageWidgetComponent->GetUserWidgetObject());
	DamageWidget->InitWidget();
	DamageWidget->SetDamageText(Damage);
	DamageWidget->PlayAnim();
}

