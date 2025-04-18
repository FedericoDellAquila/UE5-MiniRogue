#include "Utility/UtilityFunctionsLibrary.h"
#include "Core/GameplayGameMode.h"
#include "Utility/MiniRogueCheatManager.h"

AGameplayGameMode* UUtilityFunctionsLibrary::GetGameplayGameMode(UObject* WorldContextObject)
{
	return Cast<AGameplayGameMode>(WorldContextObject->GetWorld()->GetAuthGameMode());
}

UMiniRogueCheatManager* UUtilityFunctionsLibrary::GetCheatManager(UObject* WorldContextObject)
{
	return Cast<UMiniRogueCheatManager>(WorldContextObject->GetWorld()->GetFirstPlayerController()->CheatManager);
}
