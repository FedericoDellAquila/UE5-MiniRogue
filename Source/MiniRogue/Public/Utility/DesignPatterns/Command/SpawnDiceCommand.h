#pragma once
#include "CoreMinimal.h"
#include "Command.h"
#include "UObject/Object.h"
#include "SpawnDiceCommand.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MINIROGUE_API USpawnDiceCommand : public UObject, public ICommand
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta=(ExposeOnSpawn="true"))
	TSubclassOf<AActor> DiceClass;

	UPROPERTY(BlueprintReadOnly, meta=(ExposeOnSpawn="true"))
	UObject* WorldContextObject;

	UPROPERTY(BlueprintReadOnly, meta=(ExposeOnSpawn="true"))
	int32 DiceCount;

	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> DiceActors;

	virtual void Execute_Implementation() override;
	virtual void Undo_Implementation() override;

private:
	void SpawnDice();
	void DestroyDice();
};
