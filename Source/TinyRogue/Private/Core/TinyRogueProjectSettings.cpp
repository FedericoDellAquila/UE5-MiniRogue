#include "Core/TinyRogueProjectSettings.h"

UTinyRogueProjectSettings::UTinyRogueProjectSettings()
{
	CategoryName = TEXT("TinyRogue");
}

const UTinyRogueProjectSettings* UTinyRogueProjectSettings::Get()
{
	return GetDefault<UTinyRogueProjectSettings>();
}
