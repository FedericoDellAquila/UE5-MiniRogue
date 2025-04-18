#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameplayGameMode.generated.h"

UCLASS()
class MINIROGUE_API AGameplayGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure="true", Category="MiniRogueGameMode|Seed")
	const FRandomStream& GetSeed() const { return Seed; }

	UFUNCTION(BlueprintCallable, Category="MiniRogueGameMode|Seed")
	void SetSeed(FRandomStream NewSeed) { Seed = NewSeed; };

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="MiniRogueGameMode")
	void OnInitGame(const FString& MapName, const FString& Options, const FString& ErrorMessage);

private:
	FRandomStream Seed;
};
