#include "Core/TinyRoguePlayerController.h"
#include "Utility/TinyRogueCheatManager.h"
#include "Utility/DesignPatterns/StateMachine/StateMachineComponent.h"

ATinyRoguePlayerController::ATinyRoguePlayerController()
{
	CheatClass = UTinyRogueCheatManager::StaticClass();

	InputStateMachine = CreateDefaultSubobject<UStateMachineComponent>(TEXT("InputStateMachine"));
}
