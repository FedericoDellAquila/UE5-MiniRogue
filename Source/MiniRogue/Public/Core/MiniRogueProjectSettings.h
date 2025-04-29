#pragma once
#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MiniRogueProjectSettings.generated.h"

UCLASS(Config="Game", DefaultConfig, meta=(DisplayName="Mini Rogue Project Settings"))
class MINIROGUE_API UMiniRogueProjectSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UMiniRogueProjectSettings();

	UFUNCTION(BlueprintCallable, DisplayName="Get Mini Rogue Project Settings")
	static const UMiniRogueProjectSettings* Get();
};
