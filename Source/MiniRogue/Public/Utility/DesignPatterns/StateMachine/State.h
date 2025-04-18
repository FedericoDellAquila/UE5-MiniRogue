#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "State.generated.h"

class IStateMachine;

UINTERFACE(BlueprintType)
class UState : public UInterface
{
	GENERATED_BODY()
};

class MINIROGUE_API IState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Enter();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Update(float DeltaTime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Exit();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AssignStateMachine(const TScriptInterface<IStateMachine>& InStateMachine);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TScriptInterface<IStateMachine> GetStateMachine() const;
};
