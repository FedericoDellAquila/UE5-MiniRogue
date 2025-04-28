#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameplayGameMode.generated.h"

class UPhysicsWorldSimulationManager;

UCLASS()
class MINIROGUE_API AGameplayGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGameplayGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;

public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPhysicsWorldSimulationManager> PhysicsWorldSimulationManager;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<AActor> DieClass;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<AActor> FloorActor;

	UPROPERTY(BlueprintReadOnly)
	AActor* Die;

	UFUNCTION(BlueprintCallable, BlueprintPure="true", Category="MiniRogueGameMode|Seed")
	const FRandomStream& GetSeed() const { return Seed; }

	UFUNCTION(BlueprintCallable, Category="MiniRogueGameMode|Seed")
	void SetSeed(FRandomStream NewSeed) { Seed = NewSeed; };

protected:
	FRandomStream Seed;

	UFUNCTION(BlueprintImplementableEvent, Category="MiniRogueGameMode")
	void OnInitGame(const FString& MapName, const FString& Options, const FString& ErrorMessage);
};
