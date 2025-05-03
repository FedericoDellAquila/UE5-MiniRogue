#include "TinyRogue/Public/Core/TinyRogueGameMode.h"
#include "Core/RollManager/RollManager.h"
#include "Utility/Log.h"

ATinyRogueGameMode::ATinyRogueGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{}

void ATinyRogueGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	RollManager = GetWorld()->SpawnActor<ARollManager>(RollManagerClass.LoadSynchronous());
	CLOG_FATAL(IsValid(RollManager) == false, "RollManager is nullptr.");
	
	OnInitGame(MapName, Options, ErrorMessage);
}