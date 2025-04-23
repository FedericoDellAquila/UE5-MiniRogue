// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MiniRogueEditorTarget : TargetRules
{
	public MiniRogueEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("MiniRogue");
		RegisterModulesCreatedByRider();
	}

	private void RegisterModulesCreatedByRider()
	{
		ExtraModuleNames.AddRange(new[] { "MiniRogueEditor" });
	}
}