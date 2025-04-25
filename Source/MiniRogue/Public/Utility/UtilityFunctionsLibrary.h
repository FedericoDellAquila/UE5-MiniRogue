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
	static FRotator RotateFaces(int32 Value);

	UFUNCTION(BlueprintCallable)
	static void DrawCubeMeshEdges(UStaticMeshComponent* MeshComp);

	UFUNCTION(BlueprintCallable)
	static void DrawBoxEdgesFromMeshComponent(UStaticMeshComponent* MeshComp, FColor Color = FColor::Blue, float Duration = 5.0f, float Thickness = 1.0f);

	UFUNCTION(BlueprintCallable)
	static void DrawPlanes(UStaticMeshComponent* MeshComp, FColor Color = FColor::Green, float Duration = 5.0f);
};
