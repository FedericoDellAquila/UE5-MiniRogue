// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TinyRogueTarget : TargetRules
{
	public TinyRogueTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("TinyRogue");
	}
}