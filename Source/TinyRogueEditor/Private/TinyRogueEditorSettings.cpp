#include "TinyRogueEditorSettings.h"

UTinyRogueEditorSettings::UTinyRogueEditorSettings()
	: PackagedBuildDirectoryPath{FPaths::Combine(FPaths::ProjectDir(), TEXT("PackagedBuild"))}
{
	CategoryName = TEXT("TinyRogue");
}

const UTinyRogueEditorSettings* UTinyRogueEditorSettings::Get()
{
	return GetDefault<UTinyRogueEditorSettings>();
}
