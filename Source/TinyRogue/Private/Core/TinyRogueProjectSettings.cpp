#include "Core/TinyRogueProjectSettings.h"

UTinyRogueProjectSettings::UTinyRogueProjectSettings()
	: bUsePredeterminedSeed(false)
	, PredeterminedSeed(0)
{
	CategoryName = TEXT("TinyRogue");
}

const UTinyRogueProjectSettings* UTinyRogueProjectSettings::Get()
{
	return GetDefault<UTinyRogueProjectSettings>();
}
