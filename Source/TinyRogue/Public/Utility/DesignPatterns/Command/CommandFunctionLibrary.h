// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CommandFunctionLibrary.generated.h"

class UCommand;
/**
 * 
 */
UCLASS()
class TINYROGUE_API UCommandFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Adds a command to the command queue associated with the provided world context.
	 *
	 * @param WorldContextObject The object used to determine the corresponding world context.
	 *                           Typically, this is a UObject that exists within the world hierarchy.
	 * @param Command The command to enqueue.
	 * @return true if the command was enqueued successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category="TinyRogue|Command", meta=(WorldContext="WorldContextObject", DefaultToSelf="WorldContextObject"))
	static bool CommandEnqueue(UObject* WorldContextObject, UCommand* Command);

	/**
	 * Retrieves and removes the command at the front of the command queue associated with the provided world context.
	 *
	 * @param WorldContextObject The object used to determine the corresponding world context.
	 *                           Typically, this is a UObject that exists within the world hierarchy.
	 * @return true if a command was dequeued successfully, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category="TinyRogue|Command", meta=(WorldContext="WorldContextObject", DefaultToSelf="WorldContextObject"))
	static bool CommandDequeue(UObject* WorldContextObject);
};
