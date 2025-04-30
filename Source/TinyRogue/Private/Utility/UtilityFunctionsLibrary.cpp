#include "Utility/UtilityFunctionsLibrary.h"
#include "Log.h"
#include "Core/TinyRogueGameMode.h"
#include "Core/TinyRogueProjectSettings.h"
#include "Utility/TinyRogueCheatManager.h"

bool UUtilityFunctionsLibrary::GetGameplayGameMode(UObject* WorldContextObject, ATinyRogueGameMode*& OutGameMode)
{
	OutGameMode = Cast<ATinyRogueGameMode>(WorldContextObject->GetWorld()->GetAuthGameMode());
	return IsValid(OutGameMode);
}

bool UUtilityFunctionsLibrary::GetCheatManager(UObject* WorldContextObject, UTinyRogueCheatManager*& OutCheatManager)
{
	OutCheatManager = Cast<UTinyRogueCheatManager>(WorldContextObject->GetWorld()->GetFirstPlayerController()->CheatManager);
	return IsValid(OutCheatManager);
}

FTransform UUtilityFunctionsLibrary::LerpTransform(FTransform Start, FTransform End, float Alpha)
{
	const FVector NewLocation {FMath::Lerp(Start.GetLocation(), End.GetLocation(), Alpha)};
	const FQuat NewRotation {FQuat::Slerp(Start.GetRotation(), End.GetRotation(), Alpha)};
	const FVector NewScale {FMath::Lerp(Start.GetScale3D(), End.GetScale3D(), Alpha)};

	const FTransform ResultTransform(NewRotation, NewLocation, NewScale);
	return ResultTransform;
}

float UUtilityFunctionsLibrary::GetDefaultPhysicsStepDeltaTime()
{
	const IConsoleVariable* MaxFPSEditorVar {IConsoleManager::Get().FindConsoleVariable(TEXT("t.MaxFPS"))};
	float MaxFPSValue = MaxFPSEditorVar ? MaxFPSEditorVar->GetFloat() : -1.0f;

	if (MaxFPSValue <= 0.0f)
		MaxFPSValue = 120.0f;

	return 1.0f / MaxFPSValue;
}

void UUtilityFunctionsLibrary::DrawActorTransformedBoundingBox(UObject* WorldContextObject, FTransform Transform, AActor* Actor, FLinearColor Color, float Duration, float Thickness)
{
	if (IsValid(Actor) == false)
		return;

	const UWorld* World {GEngine->GetWorldFromContextObjectChecked(WorldContextObject)};
	if (IsValid(World) == false)
		return;

	// Get the local bounding box (unscaled)
	const FBox LocalBox {Actor->CalculateComponentsBoundingBoxInLocalSpace(false, true)};
	const FVector LocalCenter {LocalBox.GetCenter()};
	const FVector LocalExtent {LocalBox.GetExtent()};

	// Construct a box oriented by the provided Transform
	const FVector TransformedCenter {Transform.TransformPosition(LocalCenter)};
	const FQuat Rotation {Transform.GetRotation()};
	const FVector Scale {Transform.GetScale3D()};

	// Scale the extents correctly
	const FVector ScaledExtent {LocalExtent * Scale};

	// Draw the oriented bounding box
	DrawDebugBox(World, TransformedCenter, ScaledExtent, Rotation, Color.ToRGBE(), false, Duration, 0, Thickness);
}
