// Copyright 2025 Reflection Tree. All Rights Reserved.

using UnrealBuildTool;

public class RTreeTutorial : ModuleRules
{
	public RTreeTutorial(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
		
		PrivateDependencyModuleNames.AddRange(new string[] {
			"AIModule", "RTree", "Json", "GameplayAbilities", "GameplayTags", "GameplayTasks"
		});
	}
}
