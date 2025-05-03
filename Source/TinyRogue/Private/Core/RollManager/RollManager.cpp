#include "Core/RollManager/RollManager.h"
#include "Core/RollManager/PhysicsSimulationWorldManager.h"
#include "Core/SeededRandomFunctionsLibrary.h"

void ARollManager::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (IsValid(PhysicsWorldSimulationManager) == false)
		PhysicsWorldSimulationManager = NewObject<UPhysicsWorldSimulationManager>(this);
}

void ARollManager::ReproduceRollSimulation_Implementation(const TArray<int32>& DiceValues)
{
	
}

TArray<FPhysicsSimulationData> ARollManager::SimulateRoll_Implementation(const int32 NumDice) const
{
	return {};
}

TArray<int32> ARollManager::DetermineDiceValues_Implementation(const int32 NumDice) const
{
	if (NumDice == 0)
		return {};
	
	TArray<int32> Results;
	Results.SetNumZeroed(NumDice);
	for (int32 i = 0; i < NumDice; ++i)
		Results[i] = USeededRandomFunctionsLibrary::GetSeededIntegerInRange(GetWorld(), 1, 6);
	return Results;
}


