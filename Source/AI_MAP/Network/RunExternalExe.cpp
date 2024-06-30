// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/RunExternalExe.h"

// Sets default values
URunExternalExe::URunExternalExe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}

//// Called when the game starts or when spawned
//void ARunExternalExe::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}
//
//// Called every frame
//void ARunExternalExe::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void URunExternalExe::RunExternalExeFile()
{
	// 패키징된 프로젝트 디렉토리 얻기
	FString ProjectDir = FPaths::ProjectDir();

#if WITH_EDITOR
	// 실행 파일의 상대 경로 설정
	FString RelativePath = TEXT("../Server/Binary/Debug/GameServer.exe");
#else
	// 실행 파일의 상대 경로 설정
	FString RelativePath = TEXT("../../Server/Binary/Debug/GameServer.exe");
#endif
	// 절대 경로 생성
	FString FullPath = FPaths::Combine(ProjectDir, RelativePath);

	//프로세스ID
	uint32 OutProcessId = 0;
	ProcessId = 0;

	bool Result = false;

	// Change RelativePath to AbsolutePath
	//FString AbsolutePath = FPaths::ConvertRelativePathToFull(AppPath);
	//FString AbsolutePath = AppPath;
	FString AbsolutePath = FullPath;
	//파일실행
	FProcHandle Proc = FPlatformProcess::CreateProc(*AbsolutePath, NULL, false, false, false, &OutProcessId, -1, nullptr, nullptr);
	//FPlatformProcess::ExecElevatedProcess(*AppPath, NULL, nullptr);

	if (Proc.IsValid())
	{
		//실행되었을경우 Result에 True 리턴
		ProcessId = OutProcessId;
		Result = true;
	}
	//else {
	//	//만약 실행되지않았을경우, 관리자의 권한으로 재실행
	//	Result = FPlatformProcess::ExecElevatedProcess(*AppPath, NULL, nullptr);
	//}
}

void URunExternalExe::TerminateExternalProgram()
{
	//Uint로 ProcHandle을 받아옴
	uint32 ProcessIdUint = ProcessId;
	FProcHandle Proc = FPlatformProcess::OpenProcess(ProcessIdUint);

	//실행중인지 확인
	bool IsRunning = FPlatformProcess::IsProcRunning(Proc);
	if (IsRunning)
	{
		//실행중이라면 닫는다
		FPlatformProcess::TerminateProc(Proc);
		FPlatformProcess::CloseProc(Proc);
	}
}

void URunExternalExe::GetExecutableName(const FString& FileName, bool& result)
{
	FString Name = FileName;
	result = FPlatformProcess::IsApplicationRunning(*Name);
}

