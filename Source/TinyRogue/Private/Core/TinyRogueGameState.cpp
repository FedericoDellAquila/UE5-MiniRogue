#include "Core/TinyRogueGameState.h"

#include "Core/TimerTracker.h"

ATinyRogueGameState::ATinyRogueGameState()
	: GameSessionState(EGameSessionState::WaitingToStart)
{
	ServerWorldTimeSecondsUpdateFrequency = 1.0f;
}

void ATinyRogueGameState::PostInitProperties()
{
	Super::PostInitProperties();

	if (IsValid(TimeTracker) == false)
		TimeTracker = NewObject<UTimeTracker>(this);
}

void ATinyRogueGameState::SetGameSessionState_Implementation(const EGameSessionState NewGameSessionState)
{
	if (GameSessionState != NewGameSessionState)
	{		
		GameSessionState = NewGameSessionState;
		OnGameSessionStateChangedDelegate.Broadcast(GameSessionState);
	}
}