// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UtilityFunctionsLibrary.generated.h"

class UMiniRogueCheatManager;
class AGameplayGameMode;
/**
 * 
 */
UCLASS()
class MINIROGUE_API UUtilityFunctionsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static AGameplayGameMode* GetGameplayGameMode(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static UMiniRogueCheatManager* GetCheatManager(UObject* WorldContextObject);
};
