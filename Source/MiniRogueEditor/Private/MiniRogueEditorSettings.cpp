#include "MiniRogueEditorSettings.h"

UMiniRogueEditorSettings::UMiniRogueEditorSettings()
	: PackagedBuildDirectoryPath(TEXT("MiniRogue/PackagedBuild/Windows"))
{
	CategoryName = TEXT("MiniRogue");
}

const UMiniRogueEditorSettings* UMiniRogueEditorSettings::Get()
{
	return GetDefault<UMiniRogueEditorSettings>();
}
