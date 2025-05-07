#pragma once
#include "CoreMinimal.h"
#include "Cards/DungeonLevelsDataTableRow.h"
#include "GameFramework/GameModeBase.h"
#include "TinyRogueGameMode.generated.h"

class UDungeonLevelManagerComponent;
class UGameStateMachine;
class UTimeTracker;
class URollManagerComponent;

UCLASS()
class TINYROGUE_API ATinyRogueGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

public:
	ATinyRogueGameMode();
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<URollManagerComponent> RollManager;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UGameStateMachine> GameStateMachineClass;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UGameStateMachine> GameStateMachine;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UTimeTracker> TimeTracker;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDungeonLevelManagerComponent> DungeonLevelManager;
	
protected:
	UFUNCTION(BlueprintImplementableEvent, Category="TinyRogueGameMode")
	void OnInitGame(const FString& MapName, const FString& Options, const FString& ErrorMessage);
};
