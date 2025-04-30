#include "Core/TinyRoguePlayerController.h"
#include "Utility/TinyRogueCheatManager.h"

ATinyRoguePlayerController::ATinyRoguePlayerController()
{
	CheatClass = UTinyRogueCheatManager::StaticClass();
}
