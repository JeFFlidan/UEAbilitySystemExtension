// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MvAbilitySystem : ModuleRules
{
	public MvAbilitySystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"GameplayAbilities",
				"GameplayTags",
				"GameplayTasks",
				"ModularGameplayActors",
				"GameFeatures",
				"EnhancedInput",
				"InputCore",
				"ModularGameplay"
			}
            );
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"DeveloperSettings"
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[] {
			}
			);
	}
}
