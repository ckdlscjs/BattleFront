// Fill out your copyright notice in the Description page of Project Settings.


#include "Team_AIGameInstance.h"


UTeam_AIGameInstance::UTeam_AIGameInstance()
{
	///Script/Engine.DataTable'/Game/CSVData/CharacterStatData.CharacterStatData'
	//static ConstructorHelpers::FObjectFinder<UDataTable> DT_CharacterData(TEXT("/Script/Engine.DataTable'/Game/CharacterData.CharacterData'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_CharacterData(TEXT("/Script/Engine.DataTable'/Game/CSVData/CharacterStatData.CharacterStatData'"));
	
	if (DT_CharacterData.Succeeded())
		CharcterDataTable = DT_CharacterData.Object;
}

void UTeam_AIGameInstance::Init()
{
	Super::Init();
	/*
	if (CharcterDataTable->GetRowMap().Num() <= 0)
		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("DataTableRow Not Exist!!!!!!")), true, true, FColor::Red);
	else
		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Attack : %f"), CharcterDataTable->FindRow<FTeam_CharacterData>(*FString::FromInt(20), TEXT(""))->Attack), true, true, FColor::Red);
	*/
}

FTeam_CharacterData* UTeam_AIGameInstance::GetCharacterRowData(int32 Level)
{
	return CharcterDataTable->FindRow<FTeam_CharacterData>(*FString::FromInt(Level), TEXT(""));
}

int32 UTeam_AIGameInstance::GetSizeCharacterDataTable() const
{
	return CharcterDataTable->GetRowMap().Num();
}

