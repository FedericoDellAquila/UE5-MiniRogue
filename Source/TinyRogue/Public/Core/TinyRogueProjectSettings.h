#pragma once
#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "TinyRogueProjectSettings.generated.h"

UCLASS(Config="Game", DefaultConfig, meta=(DisplayName="Tiny Rogue Project Settings"))
class TINYROGUE_API UTinyRogueProjectSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UTinyRogueProjectSettings();
	
	UFUNCTION(BlueprintCallable, DisplayName="Get Tiny Rogue Project Settings")
	static const UTinyRogueProjectSettings* Get();

	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly)
	bool bUsePredeterminedSeed;
	
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bUsePredeterminedSeed", UIMin="0"))
	int32 PredeterminedSeed;
};
