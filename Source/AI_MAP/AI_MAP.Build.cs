// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AI_MAP : ModuleRules
{
	public AI_MAP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "UMG", "GameplayTasks", "Sockets", "Networking" });
        PrivateDependencyModuleNames.AddRange(new string[] { "ProtobufCore" });
        PrivateIncludePaths.AddRange(new string[]
        {

            "AI_MAP/",
            "AI_MAP/Network/",
            "AI_MAP/Network/Test/",
            "AI_MAP/Team_AI",
            "AI_MAP/Team_AI/Public/",
            "AI_MAP/Team_AI/Private/",
        });
    }
}
