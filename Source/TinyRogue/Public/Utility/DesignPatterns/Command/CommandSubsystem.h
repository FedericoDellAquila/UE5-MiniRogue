#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CommandSubsystem.generated.h"

class UCommand;

UCLASS()
class TINYROGUE_API UCommandSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	static UCommandSubsystem* Get(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable)
	bool Enqueue(UCommand* Command);

	UFUNCTION(BlueprintCallable)
	bool Dequeue();

	UFUNCTION(BlueprintCallable)
	void ConsumeCommands();

protected:
	UFUNCTION()
	virtual void DestroyCommand(UCommand* Command);
	
private:
	virtual bool IsTickable() const override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

	TQueue<TObjectPtr<UCommand>> CommandQueue;

	UPROPERTY()
	TObjectPtr<UCommand> CurrentCommand;
};