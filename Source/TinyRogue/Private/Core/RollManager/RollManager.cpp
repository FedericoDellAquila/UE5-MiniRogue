#include "Core/RollManager/RollManager.h"
#include "Core/RollManager/PhysicsSimulationWorldManager.h"
#include "Core/SeededRandomFunctionsLibrary.h"

UPhysicsWorldSimulationManager* URollManager::GetPhysicsWorldSimulationManager()
{
	if (IsValid(PhysicsWorldSimulationManager) == false)
		PhysicsWorldSimulationManager = NewObject<UPhysicsWorldSimulationManager>(this, TEXT("PhysicsWorldSimulationManager"));

	return PhysicsWorldSimulationManager;
}

void URollManager::ReproduceRollSimulation_Implementation(const TArray<int32>& DiceValues)
{
}

TArray<FPhysicsSimulationData> URollManager::SimulateRoll_Implementation(const int32 NumDice)
{
	return {};
}

TArray<int32> URollManager::DetermineDiceValues_Implementation(const int32 NumDice)
{
	if (NumDice == 0)
		return {};
	
	TArray<int32> Results;
	Results.SetNumZeroed(NumDice);
	for (int32 i = 0; i < NumDice; ++i)
		Results[i] = USeededRandomFunctionsLibrary::GetSeededIntegerInRange(GetWorld(), 1, 6);
	return Results;
}
