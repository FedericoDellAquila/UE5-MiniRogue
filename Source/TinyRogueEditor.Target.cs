// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TinyRogueEditorTarget : TargetRules
{
	public TinyRogueEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("TinyRogue");
		RegisterModulesCreatedByRider();
	}

	private void RegisterModulesCreatedByRider()
	{
		ExtraModuleNames.AddRange(new[] { "TinyRogueEditor" });
	}
}