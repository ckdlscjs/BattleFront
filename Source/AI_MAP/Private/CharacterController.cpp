// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterController.h"
#include "GameCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "InputCoreTypes.h"
#include "Projectile.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

void ACharacterController::BeginPlay()
{
	Super::BeginPlay();
	GameCharacter = Cast<AGameCharacter>(GetPawn());
	if (GameCharacter == nullptr)
	{
		return;
	}
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem != nullptr)
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	bDetachState = false;
	CameraSpeed = 50.f;
	bShowMouseCursor = true;
}
void ACharacterController::Tick(float DeltaTime)
{
	if (bDetachState)
	{
		EdgeCameraMove();
	}
	
	if (GameCharacter->IsFight())
	{
		
		FVector Forward = GameCharacter->GetActorForwardVector();
		FVector Temp = GetHitResultLoc() - GameCharacter->GetActorLocation();
		Temp.Z = 0;
		Temp.Normalize();
		float Degree = UKismetMathLibrary::DegAcos(FVector::DotProduct(Temp, Forward));
		FVector Cross = FVector::CrossProduct(Temp, Forward);
		if (Degree > 90)
		{
			GameCharacter->SetActorRotation(FMath::RInterpTo(GameCharacter->GetActorRotation(), Look, DeltaTime, 5.f));
		}
		if (Cross.Z < 0)
		{
			Degree *= -1;
		}
		
		GameCharacter->SetAimYaw(Degree);	
	}
}
void ACharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent != nullptr)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACharacterController::Move);
		EnhancedInputComponent->BindAction(ChangeDetachState, ETriggerEvent::Started, this, &ACharacterController::CameraDetachState);
		EnhancedInputComponent->BindAction(CameraMoveAction, ETriggerEvent::Started, this, &ACharacterController::CameraMove);
		EnhancedInputComponent->BindAction(CameraRotationAction, ETriggerEvent::Triggered, this, &ACharacterController::CameraRotation);
		EnhancedInputComponent->BindAction(CameraRotationAction, ETriggerEvent::Started, this, &ACharacterController::SaveCameraRotation);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ACharacterController::CharacterAction);
		EnhancedInputComponent->BindAction(CameraReset, ETriggerEvent::Started, this, &ACharacterController::CameraRotationReset);
		EnhancedInputComponent->BindAction(AbilityChooseAction, ETriggerEvent::Started, this, &ACharacterController::AbilityChoose);
		//Test Code
		EnhancedInputComponent->BindAction(LevelUpAction, ETriggerEvent::Started, this, &ACharacterController::LevelUp);
		EnhancedInputComponent->BindAction(ExpUpAction, ETriggerEvent::Started, this, &ACharacterController::ExpUp);
		/////////////////////
	}
}

void ACharacterController::Move(const FInputActionValue& Value)
{
	bool IsFight = GameCharacter->IsFight();
	FVector2D MovementVector = Value.Get<FVector2D>();
	FRotator Rot(0.f);
	if (IsFight == false)
	{
		GameCharacter->SetCharacterMovementRotation(true);
	}
	else
	{
		GameCharacter->SetCharacterMovementRotation(false);
	}
	GameCharacter->GetSpringArmRotator(Rot);
	FVector RightVec = UKismetMathLibrary::GetRightVector(FRotator(0, Rot.Yaw, Rot.Roll));
	GameCharacter->AddMovementInput(RightVec, MovementVector.X);
	FVector vec = GameCharacter->GetActorLocation();
	FVector ForwardVec = UKismetMathLibrary::GetForwardVector(FRotator(Rot.Pitch, Rot.Yaw, 0));
	GameCharacter->AddMovementInput(ForwardVec, MovementVector.Y);
}

void ACharacterController::CameraDetachState()
{
	bDetachState = !bDetachState;
	if (bDetachState)
	{
		GameCharacter->DetachSpringArm();
	}
	else
	{
		GameCharacter->AttachSpringArm();
	}
}

void ACharacterController::CameraMove(const FInputActionValue& Value)
{
	if (bDetachState)
	{
		FVector2D MouseLocation = Value.Get<FVector2D>();
		MouseLocation *= -1.f;
		FVector LocationX(0.f);
		FVector LocationY(0.f);
		FVector CameraLocationX(0.f);
		GameCharacter->GetSpringArmLocation(CameraLocationX);
		
		LocationX.X = CameraLocationX.X + (MouseLocation.Y * CameraSpeed);
		LocationX.Y = CameraLocationX.Y;
		LocationX.Z = CameraLocationX.Z;
		GameCharacter->SetSpringArmLocation(LocationX);

		FVector CameraLocationY(0.f);
		GameCharacter->GetSpringArmLocation(CameraLocationY);
		LocationY.X = CameraLocationY.X;
		LocationY.Y = CameraLocationY.Y + (MouseLocation.X * CameraSpeed);
		LocationY.Z = CameraLocationY.Z;
	
		GameCharacter->SetSpringArmLocation(LocationY);
	}
}

void ACharacterController::CameraRotation(const FInputActionValue& Value)
{
	FVector2D Vector = Value.Get<FVector2D>();
	if (Vector.X == 0)
	{
		GameCharacter->AddSpringArmRotaion(FRotator(0.f, 1.f, 0.f));
	}
	else
	{
		GameCharacter->AddSpringArmRotaion(FRotator(0.f, -1.f, 0.f));
	}

}

void ACharacterController::CharacterAction(const FInputActionValue& Value)
{
	bool bAttack = Value.Get<bool>();
	if (bAttack)
	{
		GameCharacter->SetFightState(true);
		AimVector = GetHitResultLoc();
		FVector CharLoc = GameCharacter->GetActorLocation();
		AimVector.Z = CharLoc.Z;
		Look = UKismetMathLibrary::FindLookAtRotation(CharLoc, AimVector);
		
		GameCharacter->Fire();
	}
}

void ACharacterController::SaveCameraRotation()
{
	GameCharacter->SaveSpringArmRotation();
}

void ACharacterController::EdgeCameraMove()
{
	if (bDetachState == false)
	{
		return;
	}
	int32 ViewportX = 0;
	int32 ViewportY = 0;
	float MouseX = 0.f;
	float MouseY = 0.f;
	float MinmumX = 2.f;
	float MinmumY = 2.f;
	float MouseMoveSpeed = 15.f;
	GetViewportSize(ViewportX,ViewportY);
	GetMousePosition(MouseX, MouseY);
	float MaximumX = ViewportX - 2.f;
	float MaximumY = ViewportY - 2.f;
	if (MouseX <= MinmumX)
	{
		FVector CamLoc(0.f);
		GameCharacter->GetSpringArmLocation(CamLoc);
		CamLoc.Y = CamLoc.Y + (MouseMoveSpeed * -1);
		GameCharacter->SetSpringArmLocation(CamLoc);
	}
	else if (MouseX >= MaximumX)
	{
		FVector CamLoc(0.f);
		GameCharacter->GetSpringArmLocation(CamLoc);
		CamLoc.Y = CamLoc.Y + MouseMoveSpeed;
		GameCharacter->SetSpringArmLocation(CamLoc);
	}
	if (MouseY <= MinmumY)
	{
		FVector CamLoc(0.f);
		GameCharacter->GetSpringArmLocation(CamLoc);
		CamLoc.X = CamLoc.X + MouseMoveSpeed;
		GameCharacter->SetSpringArmLocation(CamLoc);
	}
	else if (MouseY >= MaximumY)
	{
		FVector CamLoc(0.f);
		GameCharacter->GetSpringArmLocation(CamLoc);
		CamLoc.X = CamLoc.X + (MouseMoveSpeed * -1);
		GameCharacter->SetSpringArmLocation(CamLoc);
	}

	
}

void ACharacterController::CameraRotationReset(const FInputActionValue& Value)
{
	bool bReset = Value.Get<bool>();
	if (bReset)
	{
		GameCharacter->ResetSpringArmRotation();
	}
}

void ACharacterController::AbilityChoose(const FInputActionValue& Value)
{
	FVector Select = Value.Get<FVector>();

	FVector First(1, 0, 0);
	FVector Second(0, 1, 0);
	FVector Third(0, 0, 1);
	int index = 0;
	if (Select == First)
	{
		index = 0;
	}
	else if (Select == Second)
	{
		index = 1;
	}
	else if (Select == Third)
	{
		index = 2;
	}
	GameCharacter->SetAbility(index);
}
FVector ACharacterController::GetHitResultLoc()
{
	FVector MouseLoc(0.f);
	FVector MouseDir(0.f);
	if (DeprojectMousePositionToWorld(MouseLoc, MouseDir))
	{
		MouseDir = MouseLoc + (MouseDir * 100000);
		FHitResult Result;
		GetWorld()->LineTraceSingleByChannel(Result, MouseLoc, MouseDir, ECC_GameTraceChannel2);
		return Result.Location;
	}
	return FVector::ZeroVector;
}
/// test function
void ACharacterController::LevelUp(const FInputActionValue& Value)
{
	int32 PlayerLevel = GameCharacter->GetCharacterLevel();
	PlayerLevel++;
	GameCharacter->SetNewLevel(PlayerLevel);
}

void ACharacterController::ExpUp(const FInputActionValue& Value)
{
	bool fPush = Value.Get<bool>();
	if (fPush == true)
	{
		GameCharacter->ExpUp(17);
	}
}

