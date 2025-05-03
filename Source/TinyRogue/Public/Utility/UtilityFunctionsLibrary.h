#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UtilityFunctionsLibrary.generated.h"

class UTinyRogueCheatManager;
class ATinyRogueGameMode;

UCLASS()
class TINYROGUE_API UUtilityFunctionsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject", ExpandBoolAsExecs="ReturnValue"))
	static bool GetTinyRogueGameMode(UObject* WorldContextObject, ATinyRogueGameMode*& OutGameMode);

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject", ExpandBoolAsExecs="ReturnValue"))
	static bool GetCheatManager(UObject* WorldContextObject, UTinyRogueCheatManager*& OutCheatManager);

	UFUNCTION(BlueprintCallable)
	static FTransform LerpTransform(FTransform Start, FTransform End, float Alpha);

	UFUNCTION(BlueprintCallable)
	static void SetMaxFps(const float Value);
	
	UFUNCTION(BlueprintCallable, BlueprintPure="true")
	static float GetDefaultPhysicsStepDeltaTime();

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static void DrawActorTransformedBoundingBox(const UObject* WorldContextObject, const FTransform Transform,
		const AActor* Actor, const FLinearColor Color = FLinearColor::Blue, const float Duration = 5.0f, const float Thickness = 0.2f);
};
