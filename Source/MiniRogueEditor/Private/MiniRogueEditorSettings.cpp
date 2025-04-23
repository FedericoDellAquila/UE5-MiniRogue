// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniRogueEditorSettings.h"

UMiniRogueEditorSettings::UMiniRogueEditorSettings()
	: PackagedBuildDirectoryPath(TEXT("MiniRogue/PackagedBuild/Windows"))
{}

UMiniRogueEditorSettings* UMiniRogueEditorSettings::Get()
{
	return GetMutableDefault<UMiniRogueEditorSettings>();
}
