// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterController.h"
#include "GameCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/World.h"
#include "InputCoreTypes.h"
#include "Projectile.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon.h"
#include "Engine/EngineTypes.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"

// Server
#include "NetworkManager.h"
#include "ClientPacketHandler.h"
#include "SendBuffer.h"
#include "GameFramework/CharacterMovementComponent.h"

void ACharacterController::BeginPlay()
{
	Super::BeginPlay();
	
	// Handle_S_ENTER_GAME 에서 자신이 조종하는 캐릭터를 생성 시 주석
	GameCharacter = Cast<AGameCharacter>(GetPawn());
	//if (GameCharacter == nullptr)
	//{
	//	return;
	//}
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
	FHitResult Result;
	GetHitResultLoc(Result);

	if (GameCharacter == nullptr)
		return;

	FVector Forward = GameCharacter->GetActorForwardVector();
	FVector HitLoc = Result.Location;
	FVector ActorLoc = GameCharacter->GetActorLocation();
	FVector Temp = HitLoc - ActorLoc;
	Temp.Z = Forward.Z;
	FRotator rot = UKismetMathLibrary::FindLookAtRotation(Forward, Temp);
	GameCharacter->SetActorRotation(rot);
	GameCharacter->DesRot = GameCharacter->GetActorRotation();

	//Server
	
	if (GameCharacter->DesiredInput == FVector2D::Zero())
	{
		GameCharacter->SetMoveState(Protocol::MOVE_STATE_IDLE);
		GameCharacter->speed = 0.0f;
	}
	else
		GameCharacter->SetMoveState(Protocol::MOVE_STATE_RUN);

	MovePacketSendTimer -= DeltaTime;

	if (MovePacketSendTimer <= 0)
	{
		MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;

		Protocol::C_MOVE MovePkt;

		Protocol::PosInfo* Info = MovePkt.mutable_info();
		Info->set_object_id(GameCharacter->PlayerInfo->object_id());
		GameCharacter->DesLoc = GameCharacter->GetActorLocation();
		Info->set_x(GameCharacter->DesLoc.X);
		Info->set_y(GameCharacter->DesLoc.Y);
		Info->set_z(GameCharacter->DesLoc.Z);

		Info->set_yaw(GameCharacter->DesRot.Yaw);

		//Info->set_vx(GameCharacter->DesVel.X);
		//Info->set_vy(GameCharacter->DesVel.Y);
		//Info->set_vz(GameCharacter->DesVel.Z);

		Info->set_speed(GameCharacter->speed);
		Info->set_aimyaw(GameCharacter->GetAimYaw());
		Info->set_state(GameCharacter->GetMoveState());

		GetNetworkManager()->SendPacket(MovePkt);
	}

	//bush
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), FMC_Fade, TEXT("PlayerPos"), UKismetMathLibrary::Conv_VectorToLinearColor(GameCharacter->GetActorLocation()));
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), FMC_Fade, TEXT("BushDist"), 1000.0f);
}
void ACharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent != nullptr)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACharacterController::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ACharacterController::Move);
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
	/*UPathFollowingComponent* PFollowComp = InitNavigationControl(this);*/
	StopMovement();
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

	//Server
	GameCharacter->DesiredInput = MovementVector;
	GameCharacter->DesLoc = GameCharacter->GetActorLocation();
	GameCharacter->DesRot = GameCharacter->GetActorRotation();
	//GameCharacter->DesVel = GameCharacter->GetCharacterMovement()->GetCurrentAcceleration();
	GameCharacter->speed = GameCharacter->GetVelocity().Size();
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
		FHitResult Res;
		GetHitResultLoc(Res);
		AActor* HitActor = Res.GetActor();
		bool bIsActor = CheckActorTag(HitActor);
		if (bIsActor == true)
		{
			GameCharacter->Fire();
		}
		/*else
		{
			FVector Dir = (Res.Location - GameCharacter->GetActorLocation()).GetSafeNormal();
			GameCharacter->AddMovementInput(Dir, 1.0, false);
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Res.Location);
		}*/
		/*GameCharacter->SetFightState(true);
		AimVector = GetHitResultLoc();
		FVector Forward = GameCharacter->GetActorForwardVector();
		FVector Temp = AimVector - GameCharacter->GetActorLocation();
		Temp.Z = Forward.Z;
		Temp.Normalize();*/
		//Look = UKismetMathLibrary::MakeRotator(0, 0, Degree);
		/*Look = UKismetMathLibrary::FindLookAtRotation(Forward, Temp);*/
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
	bool isCharacterUp = GameCharacter->GetIsLevelUp();
	if (isCharacterUp == false)
	{
		return;
	}
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
void ACharacterController::GetHitResultLoc(FHitResult& Hit)
{
	//FHitResult HitTemp;
	//GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel12), true, HitTemp);
	//float HalfHeight = GameCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	//TArray<AActor*> ActorArr;
	//ActorArr.Add(GameCharacter);
	//
	//UKismetSystemLibrary::SphereTraceSingle(GetWorld(), HitTemp.TraceStart, HitTemp.TraceEnd, HalfHeight,
	//	UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel12), false, ActorArr, EDrawDebugTrace::ForOneFrame,Hit,true);
	

	FVector MouseLoc(0.f);
	FVector MouseDir(0.f);
	if (DeprojectMousePositionToWorld(MouseLoc, MouseDir))
	{
		FRotator rot = FRotator::ZeroRotator;
		//MouseLoc.RotateAngleAxis(25, FVector(0, 1, 0));
		MouseDir = MouseLoc + (MouseDir * 100000);
		
		GetWorld()->LineTraceSingleByChannel(Hit, MouseLoc, MouseDir, ECC_GameTraceChannel2);
	
	}
	return;
}
bool ACharacterController::CheckActorTag(AActor* HitActor)
{
	bool Check = true;
	//if (HitActor != nullptr)
	//{
	//	Check = (HitActor->ActorHasTag(TEXT("Enemy"))||HitActor->ActorHasTag(TEXT("Player")));
	//}

	return Check;
}
//FVector ACharacterController::GetHitResultLoc()
//{
//	FVector MouseLoc(0.f);
//	FVector MouseDir(0.f);
//	FHitResult Result;
//	//bool bHitSuccess = false;
//	//
//	//bHitSuccess = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Result);
//	//if (bHitSuccess == true)
//	//{
//	//	DrawDebugSphere(GetWorld(), Result.Location, 10, 2, FColor::Red, true, -1, 0, 2);
//	//	return Result.Location;
//	//}
//	if (DeprojectMousePositionToWorld(MouseLoc, MouseDir))
//	{
//		FRotator rot = FRotator::ZeroRotator;
//		MouseDir = MouseLoc + (MouseDir * 100000);
//		
//		GetWorld()->LineTraceSingleByChannel(Result, MouseLoc, MouseDir, ECollisionChannel::ECC_Visibility);
//		AActor* Obj = Result.GetActor();
//		if (!Obj->ActorHasTag(TEXT("Enemy")))
//		{
//			int32 a = 0;
//			a = 10;
//		}
//		DrawDebugSphere(GetWorld(), Result.Location, 3, 3, FColor::Red, true, 1, 1, 3);
//		return Result.Location;
//	}
//	return FVector::ZeroVector;
//}

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

UNetworkManager* ACharacterController::GetNetworkManager() const
{
	return GetGameInstance()->GetSubsystem<UNetworkManager>();
}

void ACharacterController::SetCharacter(AGameCharacter* player)
{
	GameCharacter = player;
}
