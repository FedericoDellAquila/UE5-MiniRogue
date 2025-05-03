#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TinyRogueGameState.generated.h"

class UTimeTracker;

UENUM(BlueprintType)
enum class EGameSessionState : uint8
{
	WaitingToStart,
	InProgress,
	Paused,
	Finished
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeTickDelegate, const FTimespan&, Time);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameSessionStateChangedDelegate, const EGameSessionState&, NewGameSessionState);

UCLASS()
class TINYROGUE_API ATinyRogueGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	virtual void PostInitProperties() override;
	
public:
	ATinyRogueGameState();

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnTimeTickDelegate OnTimeTickDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnTimeTickDelegate OnTimeStartedDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnTimeTickDelegate OnTimePausedDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnTimeTickDelegate OnTimeResumedDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnTimeTickDelegate OnTimeStoppedDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnGameSessionStateChangedDelegate OnGameSessionStateChangedDelegate;
	
	UFUNCTION(BlueprintCallable)
	EGameSessionState GetGameSessionState() const { return GameSessionState; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetGameSessionState(const EGameSessionState NewGameSessionState);

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UTimeTracker> TimeTracker;
	
	UPROPERTY()
	EGameSessionState GameSessionState;
};
