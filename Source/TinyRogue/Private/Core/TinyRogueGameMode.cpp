#include "TinyRogue/Public/Core/TinyRogueGameMode.h"

#include "Core/GameStateMachine/GameStateMachine.h"
#include "Core/TimerTracker.h"
#include "Core/RollManager/RollManager.h"
#include "GameFramework/GameMode.h"
#include "Utility/Log.h"

void ATinyRogueGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	if (IsValid(RollManager) == false)
		RollManager = NewObject<URollManager>(this, RollManagerClass.LoadSynchronous());
	
	CLOG_FATAL(IsValid(RollManager) == false, "RollManager is nullptr.");

	if (IsValid(GameStateMachine) == false)
		GameStateMachine = NewObject<UGameStateMachine>(this, GameStateMachineClass.LoadSynchronous());
	
	CLOG_FATAL(IsValid(GameStateMachine) == false, "GameStateMachine is nullptr.");
	
	if (IsValid(TimeTracker) == false)
		TimeTracker = NewObject<UTimeTracker>(this);
	
	CLOG_FATAL(IsValid(TimeTracker) == false, "TimeTracker is nullptr.");
	
	OnInitGame(MapName, Options, ErrorMessage);
}