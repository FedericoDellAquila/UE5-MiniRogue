#include "Core/MiniRogueProjectSettings.h"

UMiniRogueProjectSettings::UMiniRogueProjectSettings() {}

const UMiniRogueProjectSettings* UMiniRogueProjectSettings::Get()
{
	return GetDefault<UMiniRogueProjectSettings>();
}
