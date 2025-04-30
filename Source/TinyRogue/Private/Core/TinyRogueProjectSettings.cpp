#include "Core/TinyRogueProjectSettings.h"

UTinyRogueProjectSettings::UTinyRogueProjectSettings() {}

const UTinyRogueProjectSettings* UTinyRogueProjectSettings::Get()
{
	return GetDefault<UTinyRogueProjectSettings>();
}
