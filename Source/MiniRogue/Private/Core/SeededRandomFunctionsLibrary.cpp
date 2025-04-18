#include "Core/SeededRandomFunctionsLibrary.h"

#include "Core/GameplayGameMode.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utility/Log.h"

FRandomStream USeededRandomFunctionsLibrary::GenerateRandomSeed()
{
	FRandomStream Seed;
	Seed.GenerateNewSeed();
	return Seed;
}

int32 USeededRandomFunctionsLibrary::GetSeededIntegerInRange(UObject* WorldContextObject, int32 Min, int32 Max)
{
	int32 Value;
	if (const AGameplayGameMode* GameMode {Cast<AGameplayGameMode>(WorldContextObject->GetWorld()->GetAuthGameMode())}; IsValid(GameMode))
	{
		Value = UKismetMathLibrary::RandomIntegerInRangeFromStream(GameMode->GetSeed(), Min, Max);
	}
	else
	{
		LOG_WARNING("AGameplayGameMode not found. Fallback to non-seeded randomness.");
		Value = UKismetMathLibrary::RandomIntegerInRange(Min, Max);
	}

	return Value;
}

float USeededRandomFunctionsLibrary::GetSeededFloatInRange(UObject* WorldContextObject, float Min, float Max)
{
	float Value;
	if (const AGameplayGameMode* GameMode = Cast<AGameplayGameMode>(WorldContextObject->GetWorld()->GetAuthGameMode()))
	{
		Value = UKismetMathLibrary::RandomFloatInRangeFromStream(GameMode->GetSeed(), Min, Max);
	}
	else
	{
		LOG_WARNING("AGameplayGameMode not found. Fallback to non-seeded randomness.");
		Value = UKismetMathLibrary::RandomFloatInRange(Min, Max);
	}

	return Value;
}

bool USeededRandomFunctionsLibrary::GetSeededBool(UObject* WorldContextObject)
{
	bool bValue;
	if (const AGameplayGameMode* GameMode = Cast<AGameplayGameMode>(WorldContextObject->GetWorld()->GetAuthGameMode()))
	{
		bValue = UKismetMathLibrary::RandomBoolFromStream(GameMode->GetSeed());
	}
	else
	{
		LOG_WARNING("AGameplayGameMode not found. Fallback to non-seeded randomness.");
		bValue = UKismetMathLibrary::RandomBool();
	}

	return bValue;
}

FVector USeededRandomFunctionsLibrary::GetSeededUnitVectorInConeInDegrees(UObject* WorldContextObject, FVector ConeDirection, float ConeHalfAngleInDegrees)
{
	FVector Value;
	if (const AGameplayGameMode* GameMode {Cast<AGameplayGameMode>(WorldContextObject->GetWorld()->GetAuthGameMode())}; IsValid(GameMode))
	{
		Value = UKismetMathLibrary::RandomUnitVectorInConeInDegreesFromStream(GameMode->GetSeed(), ConeDirection, ConeHalfAngleInDegrees);
	}
	else
	{
		LOG_WARNING("AGameplayGameMode not found. Fallback to non-seeded randomness.");
		Value = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(ConeDirection, ConeHalfAngleInDegrees);
	}

	return Value;
}

FVector USeededRandomFunctionsLibrary::GetSeededUnitVector(UObject* WorldContextObject)
{
	FVector Value;
	if (const AGameplayGameMode* GameMode {Cast<AGameplayGameMode>(WorldContextObject->GetWorld()->GetAuthGameMode())}; IsValid(GameMode))
	{
		Value = UKismetMathLibrary::RandomUnitVectorFromStream(GameMode->GetSeed());
	}
	else
	{
		LOG_WARNING("AGameplayGameMode not found. Fallback to non-seeded randomness.");
		Value = UKismetMathLibrary::RandomUnitVector();
	}

	return Value;
}

FRotator USeededRandomFunctionsLibrary::GetSeededRotator(UObject* WorldContextObject, bool bRoll/*= true*/)
{
	FRotator Value;
	if (const AGameplayGameMode* GameMode = Cast<AGameplayGameMode>(WorldContextObject->GetWorld()->GetAuthGameMode()))
	{
		Value = UKismetMathLibrary::RandomRotatorFromStream(GameMode->GetSeed(), bRoll);
	}
	else
	{
		LOG_WARNING("AGameplayGameMode not found. Fallback to non-seeded randomness.");
		Value = UKismetMathLibrary::RandomRotator();
	}

	return Value;
}
