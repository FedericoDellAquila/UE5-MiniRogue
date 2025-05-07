#include "Core/TinyRoguePlayerController.h"

#include "Core/GameStateMachine/StateMachineComponent.h"
#include "Utility/TinyRogueCheatManager.h"

ATinyRoguePlayerController::ATinyRoguePlayerController()
{
	CheatClass = UTinyRogueCheatManager::StaticClass();
	InputStateMachine = CreateDefaultSubobject<UStateMachineComponent>(TEXT("InputStateMachine"));
}
