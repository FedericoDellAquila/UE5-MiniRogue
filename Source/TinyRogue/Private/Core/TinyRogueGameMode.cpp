#include "TinyRogue/Public/Core/TinyRogueGameMode.h"
#include "Components/PhysicsSimulationWorldManager.h"

ATinyRogueGameMode::ATinyRogueGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Die(nullptr)
{}

void ATinyRogueGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	if (IsValid(PhysicsWorldSimulationManager) == false)
	{
		PhysicsWorldSimulationManager = NewObject<UPhysicsWorldSimulationManager>(this);
	}

	OnInitGame(MapName, Options, ErrorMessage);
}