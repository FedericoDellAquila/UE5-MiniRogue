#include "Core/GamePlayerController.h"
#include "Utility/MiniRogueCheatManager.h"

AGamePlayerController::AGamePlayerController()
{
	CheatClass = UMiniRogueCheatManager::StaticClass();
}
