#include "TinyRogue/Public/Core/TinyRogueGameMode.h"
#include "Core/RollManager/RollManager.h"
#include "Utility/Log.h"

void ATinyRogueGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	if (IsValid(RollManager) == false)
		RollManager = NewObject<URollManager>(this, RollManagerClass.LoadSynchronous());
	
	CLOG_FATAL(IsValid(RollManager) == false, "RollManager is nullptr.");
	
	OnInitGame(MapName, Options, ErrorMessage);
}