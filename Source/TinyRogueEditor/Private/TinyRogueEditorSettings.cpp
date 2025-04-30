#include "TinyRogueEditorSettings.h"

UTinyRogueEditorSettings::UTinyRogueEditorSettings()
	: PackagedBuildDirectoryPath(TEXT("TinyRogue/PackagedBuild/Windows"))
{
	CategoryName = TEXT("TinyRogue");
}

const UTinyRogueEditorSettings* UTinyRogueEditorSettings::Get()
{
	return GetDefault<UTinyRogueEditorSettings>();
}
