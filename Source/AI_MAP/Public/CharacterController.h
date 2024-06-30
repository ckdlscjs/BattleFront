// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "CharacterController.generated.h"

class UNetworkManager;
class UMaterialParameterCollection;
/**
 * 
 */
UCLASS()
class AI_MAP_API ACharacterController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	void DisableController();
	bool IsCharaterDeath();
	void SetMyMouseCursor();
	virtual void BeginDestroy() override;

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
	
	void Move(const FInputActionValue& Value);
	void CameraDetachState();
	void CameraMove(const FInputActionValue& Value);
	void CameraRotation(const FInputActionValue& Value);
	void CharacterAction(const FInputActionValue& Value);
	void SaveCameraRotation();
	void EdgeCameraMove();
	void CameraRotationReset(const FInputActionValue& Value);
	void AbilityChoose(const FInputActionValue& Value);
	void GetHitResultLoc(FHitResult& Hit);
	bool CheckActorTag(AActor* HitActor);

	void WorldMapWidgetToggle();

	/*Test Code*/
	/*
	void LevelUp(const FInputActionValue& Value);
	void ExpUp(const FInputActionValue& Value);
	*/
	/******************************************/
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class  UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* ChangeDetachState;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* CameraMoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* CameraRotationAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* CameraReset; // Rotation Reset
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		class  UInputAction* AbilityChooseAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		class  UInputAction* WorldMapToggle;
	UPROPERTY(EditAnywhere, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		class USoundBase* AbilityChooseSound;
	UPROPERTY(EditAnywhere, Category = "Combat")
		TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY()
		FVector AimVector;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
		float ShootCoolTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
		bool bCanShoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = "true"))
		bool bIsPlayerDeath;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UUserWidget> MouseCursorClass;
	// Test Code
		/*
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		class  UInputAction* LevelUpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		class  UInputAction* ExpUpAction;
		*/
	/*****************************************/
	UPROPERTY(EditAnywhere)
	class AGameCharacter* GameCharacter;
	UPROPERTY()
	bool bDetachState;
	UPROPERTY()
	float CameraSpeed;

	FRotator Look;

	//using bush
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bush", meta = (AllowPrivateAccess = "true"))
	UMaterialParameterCollection* FMC_Fade;
public:
	UNetworkManager* GetNetworkManager() const;
	const float MOVE_PACKET_SEND_DELAY = 0.0f;
	float MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;
	void SetCharacter(AGameCharacter* player);
	AActor* GetGameCharacter() const;
};
