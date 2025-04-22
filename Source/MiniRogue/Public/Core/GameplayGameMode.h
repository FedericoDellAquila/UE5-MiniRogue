#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameplayGameMode.generated.h"

USTRUCT(BlueprintType)
struct FDiceAnimationPath
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TArray<FTransform> Transforms; // Transforms at each step of the simulation

	UPROPERTY(BlueprintReadOnly)
	int32 DieID; // ID to identify which die this path belongs to
};


UCLASS()
class MINIROGUE_API AGameplayGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;

public:
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<AActor> DieClass;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<AActor> FloorActor;

	UPROPERTY(BlueprintReadOnly)
	AActor* Die;

	UFUNCTION(BlueprintCallable)
	void SimulateRoll(TArray<FDiceAnimationPath>& OutAnimationPaths, int32 NumDice);

	UFUNCTION(BlueprintCallable, BlueprintPure="true", Category="MiniRogueGameMode|Seed")
	const FRandomStream& GetSeed() const { return Seed; }

	UFUNCTION(BlueprintCallable, Category="MiniRogueGameMode|Seed")
	void SetSeed(FRandomStream NewSeed) { Seed = NewSeed; };

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="MiniRogueGameMode")
	void OnInitGame(const FString& MapName, const FString& Options, const FString& ErrorMessage);

private:
	FRandomStream Seed;

	UPROPERTY()
	TObjectPtr<UWorld> SimWorld;

	TArray<FDiceAnimationPath> DiceAnimationPaths;
};
