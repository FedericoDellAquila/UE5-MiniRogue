#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CommandSubsystem.generated.h"

class UCommand;

UCLASS()
class TINYROGUE_API UCommandSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UCommandSubsystem* Get(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable)
	bool Enqueue(UCommand* Command, UCommand*& OutCommand);

	UFUNCTION(BlueprintCallable)
	bool Dequeue(UCommand*& OutCommand);
	
private:
	TQueue<UCommand*> CommandQueue;
};
