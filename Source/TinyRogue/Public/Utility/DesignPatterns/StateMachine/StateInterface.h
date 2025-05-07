#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StateInterface.generated.h"

class IStateInterface;
class IStateMachineInterface;

UINTERFACE(BlueprintType)
class UStateInterface : public UInterface
{
	GENERATED_BODY()

public:
	static bool CreateStateObject(AActor* Outer, const TSoftClassPtr<UObject> Class, TScriptInterface<IStateInterface>& OutState);
};

class TINYROGUE_API IStateInterface
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
	void AssignStateMachine(const TScriptInterface<IStateMachineInterface>& InStateMachine);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TScriptInterface<IStateMachineInterface> GetStateMachine() const;
};
