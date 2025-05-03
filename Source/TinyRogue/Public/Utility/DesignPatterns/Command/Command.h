#pragma once
#include "CoreMinimal.h"
#include "Core/WorldObject.h"
#include "Command.generated.h"

class UCommandSubsystem;

UCLASS(Blueprintable, BlueprintType, Abstract)
class TINYROGUE_API UCommand : public UWorldObject
{
	GENERATED_BODY()

public:
	UCommand();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="TinyRogue|Command")
	void Execute();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="TinyRogue|Command")
	void Finish();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="TinyRogue|Command")
	void Undo();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="TinyRogue|Command")
	bool IsBeingExecuted() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="TinyRogue|Command")
	UCommandSubsystem* GetCommandSubsystem() const;
	
protected:
	bool bIsBeingExecuted;
};
