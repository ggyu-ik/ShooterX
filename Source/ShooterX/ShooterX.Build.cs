// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ShooterX : ModuleRules
{
	public ShooterX(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
			//AI
			"AIModule", "NavigationSystem", "GameplayTasks",
			//UI
			"UMG",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"ShooterXPlayGround",
		});
	}
}
