#include "Core/SeededRandomFunctionsLibrary.h"
#include "Core/TinyRogueGameInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utility/Log.h"

int32 USeededRandomFunctionsLibrary::GetSeededIntegerInRange(UObject* WorldContextObject, const int32 Min, const int32 Max)
{
	int32 Value {};
	if (const UTinyRogueGameInstance* GameMode {Cast<UTinyRogueGameInstance>(WorldContextObject->GetWorld()->GetGameInstance())}; IsValid(GameMode))
		Value = UKismetMathLibrary::RandomIntegerInRangeFromStream(GameMode->GetSeed(), Min, Max);
	else
		LOG_ERROR("{0} not found.", UTinyRogueGameInstance::StaticClass()->GetName());
	
	return Value;
}

float USeededRandomFunctionsLibrary::GetSeededFloatInRange(UObject* WorldContextObject, const float Min, const float Max)
{
	float Value {};
	if (const UTinyRogueGameInstance* GameMode {Cast<UTinyRogueGameInstance>(WorldContextObject->GetWorld()->GetGameInstance())}; IsValid(GameMode))
		Value = UKismetMathLibrary::RandomFloatInRangeFromStream(GameMode->GetSeed(), Min, Max);
	else
		LOG_ERROR("{0} not found.", UTinyRogueGameInstance::StaticClass()->GetName());

	return Value;
}

bool USeededRandomFunctionsLibrary::GetSeededBool(UObject* WorldContextObject)
{
	bool bValue {};
	if (const UTinyRogueGameInstance* GameMode {Cast<UTinyRogueGameInstance>(WorldContextObject->GetWorld()->GetGameInstance())}; IsValid(GameMode))
		bValue = UKismetMathLibrary::RandomBoolFromStream(GameMode->GetSeed());
	else
		LOG_ERROR("{0} not found.", UTinyRogueGameInstance::StaticClass()->GetName());

	return bValue;
}

FVector USeededRandomFunctionsLibrary::GetSeededUnitVectorInConeInDegrees(UObject* WorldContextObject, const FVector ConeDirection, const float ConeHalfAngleInDegrees)
{
	FVector Value {};
	if (const UTinyRogueGameInstance* GameMode {Cast<UTinyRogueGameInstance>(WorldContextObject->GetWorld()->GetGameInstance())}; IsValid(GameMode))
		Value = UKismetMathLibrary::RandomUnitVectorInConeInDegreesFromStream(GameMode->GetSeed(), ConeDirection, ConeHalfAngleInDegrees);
	else
		LOG_ERROR("{0} not found.", UTinyRogueGameInstance::StaticClass()->GetName());

	return Value;
}

FVector USeededRandomFunctionsLibrary::GetSeededUnitVector(UObject* WorldContextObject)
{
	FVector Value {};
	if (const UTinyRogueGameInstance* GameMode {Cast<UTinyRogueGameInstance>(WorldContextObject->GetWorld()->GetGameInstance())}; IsValid(GameMode))
		Value = UKismetMathLibrary::RandomUnitVectorFromStream(GameMode->GetSeed());
	else
		LOG_ERROR("{0} not found.", UTinyRogueGameInstance::StaticClass()->GetName());

	return Value;
}

FRotator USeededRandomFunctionsLibrary::GetSeededRotator(UObject* WorldContextObject, const bool bRoll/*= true*/)
{
	FRotator Value {};
	if (const UTinyRogueGameInstance* GameMode {Cast<UTinyRogueGameInstance>(WorldContextObject->GetWorld()->GetGameInstance())}; IsValid(GameMode))
		Value = UKismetMathLibrary::RandomRotatorFromStream(GameMode->GetSeed(), bRoll);
	else
		LOG_ERROR("{0} not found.", UTinyRogueGameInstance::StaticClass()->GetName());

	return Value;
}
