// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SeededRandomFunctionsLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MINIROGUE_API USeededRandomFunctionsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="SeededRandomFunctionsLibrary")
	static FRandomStream GenerateRandomSeed();

	UFUNCTION(BlueprintCallable, BlueprintPure="true", Category="SeededRandomFunctionsLibrary", meta=(WorldContext="WorldContextObject"))
	static int32 GetSeededIntegerInRange(UObject* WorldContextObject, int32 Min, int32 Max);

	UFUNCTION(BlueprintCallable, BlueprintPure="true", Category="SeededRandomFunctionsLibrary", meta=(WorldContext="WorldContextObject"))
	static float GetSeededFloatInRange(UObject* WorldContextObject, float Min, float Max);

	UFUNCTION(BlueprintCallable, BlueprintPure="true", Category="SeededRandomFunctionsLibrary", meta=(WorldContext="WorldContextObject"))
	static bool GetSeededBool(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintPure="true", Category="SeededRandomFunctionsLibrary", meta=(WorldContext="WorldContextObject"))
	static FVector GetSeededUnitVectorInConeInDegrees(UObject* WorldContextObject, FVector ConeDirection, float ConeHalfAngleInDegrees);

	UFUNCTION(BlueprintCallable, BlueprintPure="true", Category="SeededRandomFunctionsLibrary", meta=(WorldContext="WorldContextObject"))
	static FVector GetSeededUnitVector(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintPure="true", Category="SeededRandomFunctionsLibrary", meta=(WorldContext="WorldContextObject"))
	static FRotator GetSeededRotator(UObject* WorldContextObject, bool bRoll = true);
};
