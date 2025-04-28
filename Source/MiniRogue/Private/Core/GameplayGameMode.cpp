#include "MiniRogue/Public/Core/GameplayGameMode.h"
#include "Components/PhysicsSimulationWorldManager.h"

AGameplayGameMode::AGameplayGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Die(nullptr)
{}

void AGameplayGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	if (IsValid(PhysicsWorldSimulationManager) == false)
	{
		PhysicsWorldSimulationManager = NewObject<UPhysicsWorldSimulationManager>(this);
	}

	OnInitGame(MapName, Options, ErrorMessage);
}

APawn* AGameplayGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	return SpawnDefaultPawnAtTransform(NewPlayer, StartSpot->GetActorTransform());
}
