#include "Utility/MiniRogueCheatManager.h"

#include "Log.h"
#include "Utility/UtilityFunctionsLibrary.h"

static FAutoConsoleCommandWithWorldAndArgs TestCommand(
	TEXT("MiniRogueCheatManager.SimulateDiceRoll"),
	TEXT("Simulates a die roll based on a string of space-separated values. Each numeric value in the provided string is parsed and processed as an individual dice roll result. Non-numeric values are ignored."),
	FConsoleCommandWithWorldAndArgsDelegate::CreateStatic([](const TArray<FString>& Values, UWorld* World) -> void
		{
			const UMiniRogueCheatManager* CheatManager {UUtilityFunctionsLibrary::GetCheatManager(World)};
			if (!IsValid(CheatManager))
			{
				LOG_ERROR("CheatManager is invalid!")
				return;
			}
			CheatManager->SimulateDiceRoll(Values);
		}
	),
	ECVF_Cheat
);

void UMiniRogueCheatManager::SimulateDiceRoll(const TArray<FString>& Values) const
{
	if (Values.IsEmpty())
	{
		return;
	}

	TArray<int32> NumericValues {};
	NumericValues.Reserve(Values.Num());

	for (int32 i = 0; i < Values.Num(); i++)
	{
		const FString& CurrentValue {Values[i]};
		if (CurrentValue.IsNumeric() == false)
		{
			continue;
		}

		NumericValues.Emplace(FCString::Atoi(*CurrentValue));
	}

	// TODO: inform the system about the result
}
