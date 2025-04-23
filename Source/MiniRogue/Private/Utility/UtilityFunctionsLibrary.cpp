#include "Utility/UtilityFunctionsLibrary.h"
#include "Core/GameplayGameMode.h"
#include "PhysicsEngine/PhysicsSettings.h"
#include "Utility/MiniRogueCheatManager.h"

bool UUtilityFunctionsLibrary::GetGameplayGameMode(UObject* WorldContextObject, AGameplayGameMode*& OutGameMode)
{
	OutGameMode = Cast<AGameplayGameMode>(WorldContextObject->GetWorld()->GetAuthGameMode());
	return IsValid(OutGameMode);
}

bool UUtilityFunctionsLibrary::GetCheatManager(UObject* WorldContextObject, UMiniRogueCheatManager*& OutCheatManager)
{
	OutCheatManager = Cast<UMiniRogueCheatManager>(WorldContextObject->GetWorld()->GetFirstPlayerController()->CheatManager);
	return IsValid(OutCheatManager);
}

float UUtilityFunctionsLibrary::GetPhysicsStepDeltaTime()
{
	const UPhysicsSettings* PhysicsSettings {UPhysicsSettings::Get()};
	return PhysicsSettings->MaxSubstepDeltaTime;
}
