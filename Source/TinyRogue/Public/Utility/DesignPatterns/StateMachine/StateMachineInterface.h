#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StateMachineInterface.generated.h"

class IStateInterface;

UINTERFACE(BlueprintType)
class UStateMachineInterface : public UInterface
{
	GENERATED_BODY()
};

class TINYROGUE_API IStateMachineInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TScriptInterface<IStateInterface> GetCurrentState() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TScriptInterface<IStateInterface> GetPreviousState() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ChangeState(const TScriptInterface<IStateInterface>& NewState);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Update(float DeltaTime);
};
