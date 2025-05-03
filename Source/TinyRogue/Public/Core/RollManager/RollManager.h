#pragma once
#include "CoreMinimal.h"
#include "PhysicsSimulationWorldManager.h"
#include "RollManager.generated.h"

UCLASS(Blueprintable, BlueprintType)
class TINYROGUE_API ARollManager : public AActor
{
	GENERATED_BODY()

protected:
	virtual void PostInitializeComponents() override;
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure="false")
	TArray<int32> DetermineDiceValues(const int32 NumDice) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure="false")
	TArray<FPhysicsSimulationData> SimulateRoll(const int32 NumDice) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure="false")
	void ReproduceRollSimulation(const TArray<int32>& DiceValues);

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPhysicsWorldSimulationManager> PhysicsWorldSimulationManager;
};
