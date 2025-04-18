#include "MiniRogue/Public/Core/GameplayGameMode.h"

void AGameplayGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	OnInitGame(MapName, Options, ErrorMessage);
}

APawn* AGameplayGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	return SpawnDefaultPawnAtTransform(NewPlayer, StartSpot->GetActorTransform());
}
