// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
#include "UObject/NoExportTypes.h"
#include "RunExternalExe.generated.h"

UCLASS()
class AI_MAP_API URunExternalExe : public UObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	URunExternalExe();

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

public:
	//// Called every frame
	//virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "ControlExternalFiles")
	void RunExternalExeFile();

	UFUNCTION(BlueprintCallable, Category = "ControlExternalFiles")
	void TerminateExternalProgram();

	UFUNCTION(BlueprintCallable, Category = "ControlExternalFiles")
	void GetExecutableName(const FString& FileName, bool& result);
public:
	int64 ProcessId;
};
