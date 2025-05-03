#pragma once
#include "CoreMinimal.h"
#include "PhysicsSimulationWorldManager.h"
#include "Core/WorldObject.h"
#include "RollManager.generated.h"

UCLASS(Blueprintable, BlueprintType)
class TINYROGUE_API URollManager : public UWorldObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure="false")
	TArray<int32> DetermineDiceValues(const int32 NumDice);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure="false")
	TArray<FPhysicsSimulationData> SimulateRoll(const int32 NumDice);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReproduceRollSimulation(const TArray<int32>& DiceValues);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UPhysicsWorldSimulationManager* GetPhysicsWorldSimulationManager();
	
protected:
	UPROPERTY()
	TObjectPtr<UPhysicsWorldSimulationManager> PhysicsWorldSimulationManager;
};
