#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UtilityFunctionsLibrary.generated.h"

class UMiniRogueCheatManager;
class AGameplayGameMode;

UCLASS()
class MINIROGUE_API UUtilityFunctionsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject", ExpandBoolAsExecs="ReturnValue"))
	static bool GetGameplayGameMode(UObject* WorldContextObject, AGameplayGameMode*& OutGameMode);

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject", ExpandBoolAsExecs="ReturnValue"))
	static bool GetCheatManager(UObject* WorldContextObject, UMiniRogueCheatManager*& OutCheatManager);

	UFUNCTION(BlueprintCallable, BlueprintPure="true")
	static float GetPhysicsStepDeltaTime();

	UFUNCTION(BlueprintCallable)
	static void DrawBoxEdgesFromMeshComponent(UWorld* World, FTransform Transform, UStaticMeshComponent* MeshComp, FLinearColor Color = FLinearColor::Blue, float Duration = 5.0f, float Thickness = 0.2f);
};
