#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SeededRandomFunctionsLibrary.generated.h"

UCLASS()
class TINYROGUE_API USeededRandomFunctionsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure="true", Category="SeededRandomFunctionsLibrary", meta=(WorldContext="WorldContextObject", DefaultToSelf="WorldContextObject"))
	static int32 GetSeededIntegerInRange(UObject* WorldContextObject, int32 Min, int32 Max);

	UFUNCTION(BlueprintCallable, BlueprintPure="true", Category="SeededRandomFunctionsLibrary", meta=(WorldContext="WorldContextObject", DefaultToSelf="WorldContextObject"))
	static float GetSeededFloatInRange(UObject* WorldContextObject, float Min, float Max);

	UFUNCTION(BlueprintCallable, BlueprintPure="true", Category="SeededRandomFunctionsLibrary", meta=(WorldContext="WorldContextObject", DefaultToSelf="WorldContextObject"))
	static bool GetSeededBool(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintPure="true", Category="SeededRandomFunctionsLibrary", meta=(WorldContext="WorldContextObject", DefaultToSelf="WorldContextObject"))
	static FVector GetSeededUnitVectorInConeInDegrees(UObject* WorldContextObject, FVector ConeDirection, float ConeHalfAngleInDegrees);

	UFUNCTION(BlueprintCallable, BlueprintPure="true", Category="SeededRandomFunctionsLibrary", meta=(WorldContext="WorldContextObject", DefaultToSelf="WorldContextObject"))
	static FVector GetSeededUnitVector(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintPure="true", Category="SeededRandomFunctionsLibrary", meta=(WorldContext="WorldContextObject", DefaultToSelf="WorldContextObject"))
	static FRotator GetSeededRotator(UObject* WorldContextObject, bool bRoll = true);
};
