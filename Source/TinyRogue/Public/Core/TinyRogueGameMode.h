#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TinyRogueGameMode.generated.h"

class UPhysicsWorldSimulationManager;

UCLASS()
class TINYROGUE_API ATinyRogueGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATinyRogueGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPhysicsWorldSimulationManager> PhysicsWorldSimulationManager;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<AActor> DieClass;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<AActor> FloorActor;

	UPROPERTY(BlueprintReadOnly)
	AActor* Die;

	UFUNCTION(BlueprintCallable, BlueprintPure="true", Category="TinyRogueGameMode|Seed")
	const FRandomStream& GetSeed() const { return Seed; }

	UFUNCTION(BlueprintCallable, Category="TinyRogueGameMode|Seed")
	void SetSeed(FRandomStream NewSeed) { Seed = NewSeed; };

protected:
	FRandomStream Seed;

	UFUNCTION(BlueprintImplementableEvent, Category="TinyRogueGameMode")
	void OnInitGame(const FString& MapName, const FString& Options, const FString& ErrorMessage);
};
