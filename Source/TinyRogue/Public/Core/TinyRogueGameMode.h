#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TinyRogueGameMode.generated.h"

class ARollManager;

UCLASS()
class TINYROGUE_API ATinyRogueGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	explicit ATinyRogueGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

public:
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<ARollManager> RollManagerClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ARollManager> RollManager;
	
	UFUNCTION(BlueprintCallable, BlueprintPure="true", Category="TinyRogueGameMode|Seed")
	const FRandomStream& GetSeed() const { return Seed; }

	UFUNCTION(BlueprintCallable, Category="TinyRogueGameMode|Seed")
	void SetSeed(const FRandomStream NewSeed) { Seed = NewSeed; };

protected:
	FRandomStream Seed;
	
	UFUNCTION(BlueprintImplementableEvent, Category="TinyRogueGameMode")
	void OnInitGame(const FString& MapName, const FString& Options, const FString& ErrorMessage);
};
