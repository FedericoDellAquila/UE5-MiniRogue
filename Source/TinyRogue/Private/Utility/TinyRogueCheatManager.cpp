#include "Utility/TinyRogueCheatManager.h"

#include "Log.h"
#include "Core/RollManager/RollManagerComponent.h"
#include "Core/SeededRandomFunctionsLibrary.h"
#include "Core/TinyRogueGameMode.h"
#include "Utility/UtilityFunctionsLibrary.h"

static FAutoConsoleCommandWithWorldAndArgs TestCommand(
	TEXT("TinyRogueCheatManager.SimulateDiceRoll"),
	TEXT("Simulates a die roll based on a string of space-separated values. Each numeric value in the provided string is parsed and processed as an individual dice roll result. Non-numeric values are ignored. A 0 will use the seed to determine the value."),
	FConsoleCommandWithWorldAndArgsDelegate::CreateStatic([](const TArray<FString>& Values, UWorld* World) -> void
		{
			UTinyRogueCheatManager* CheatManager {};
			if (UUtilityFunctionsLibrary::GetCheatManager(World, CheatManager) == false)
			{
				LOG_ERROR("CheatManager is invalid!")
				return;
			}
			CheatManager->SimulateDiceRoll(Values);
		}
	),
	ECVF_Cheat
);

void UTinyRogueCheatManager::SimulateDiceRoll(const TArray<FString>& Values)
{
	if (Values.IsEmpty())
		return;

	TArray<int32> NumericValues {};
	NumericValues.Reserve(Values.Num());

	for (int32 i = 0; i < Values.Num(); i++)
	{
		const FString& CurrentValue {Values[i]};
		if (CurrentValue.IsNumeric() == false)
			continue;

		int32 Value {FCString::Atoi(*CurrentValue)};
		if (Value < 1)
			Value = USeededRandomFunctionsLibrary::GetSeededIntegerInRange(GetWorld(), 1, 6);
		
		NumericValues.Emplace(Value);
	}

	ATinyRogueGameMode* GameMode;
	if (UUtilityFunctionsLibrary::GetTinyRogueGameMode(this, GameMode) == false)
	{
		LOG_ERROR("GameMode is not of type {0}", ATinyRogueGameMode::StaticClass()->GetName());
		return;
	}
	GameMode->RollManager->ReproduceRollSimulation(NumericValues);
}
