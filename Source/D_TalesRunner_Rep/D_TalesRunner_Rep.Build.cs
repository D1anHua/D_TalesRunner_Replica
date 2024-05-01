// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class D_TalesRunner_Rep : ModuleRules
{
	public D_TalesRunner_Rep(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreOnline",
			"CoreUObject", 
			"ApplicationCore",
			"Engine", 
			"InputCore",
			"SlateCore",
			"EnhancedInput",
			"Niagara",
			"ControlFlows",
			"CommonOnlineTools",
			"GameplayAbilities",
			"GameplayTasks",
			"GameplayTags",
			"CommonUI",
			"CommonInput",
			"NetCore",
			"AnimGraphRuntime",
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
