#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Command.generated.h"

UINTERFACE(BlueprintType)
class UCommand : public UInterface
{
	GENERATED_BODY()
};

class TINYROGUE_API ICommand
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Execute();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Undo();
};
