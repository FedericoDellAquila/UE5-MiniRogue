#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StateMachine.generated.h"

class IState;

UINTERFACE(BlueprintType)
class UStateMachine : public UInterface
{
	GENERATED_BODY()
};

class TINYROGUE_API IStateMachine
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TScriptInterface<IState> GetCurrentState() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TScriptInterface<IState> GetPreviousState() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ChangeState(const TScriptInterface<IState>& NewState);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Update(float DeltaTime);
};
