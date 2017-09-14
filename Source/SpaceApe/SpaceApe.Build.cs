// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SpaceApe : ModuleRules
{
	public SpaceApe(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "Slate", "SlateCore", "OnlineSubsystem", "OnlineSubsystemUtils", "AIModule", "GameplayTasks" });

        DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");


        //refresh your visual studio files from editor after adding this to avoid weird redline errors
        //linking steamworks
        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Linux) || (Target.Platform == UnrealTargetPlatform.Mac)) {
            PublicDependencyModuleNames.AddRange(new string[] { "Steamworks" });
        }
        // AddThirdPartyPrivateStaticDependencies(Target, "Steamworks");
        // PrivateDependencyModuleNames.Add("Steamworks");
        // Uncomment if you are using online features
        PrivateDependencyModuleNames.Add("OnlineSubsystem");
    }
}
