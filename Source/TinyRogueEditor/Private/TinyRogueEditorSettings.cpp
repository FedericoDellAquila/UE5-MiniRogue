#include "TinyRogueEditorSettings.h"

UTinyRogueEditorSettings::UTinyRogueEditorSettings()
	: PackagedBuildDirectoryPath{FPaths::Combine(FPaths::ProjectDir(), TEXT("PackagedBuild"))}
	, TrelloUrl{TEXT("https://trello.com/b/e9QI4VD0/tinyrogue")}
{
	CategoryName = TEXT("TinyRogue");
}

const UTinyRogueEditorSettings* UTinyRogueEditorSettings::Get()
{
	return GetDefault<UTinyRogueEditorSettings>();
}
