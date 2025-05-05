#include "Utility/UtilityFunctionsLibrary.h"

#include "Log.h"
#include "Core/TinyRogueGameInstance.h"
#include "Core/TinyRogueGameMode.h"
#include "Utility/TinyRogueCheatManager.h"

FString UUtilityFunctionsLibrary::Conv_TimespanToString(const FTimespan& Timespan)
{
	const FString TimespanString {FString::Printf(TEXT("%02i:%02i:%02i"),
		FMath::Abs(Timespan.GetHours()),
		FMath::Abs(Timespan.GetHours()),
		FMath::Abs(Timespan.GetSeconds()))};
	return TimespanString;
}

bool UUtilityFunctionsLibrary::GetTinyRogueGameInstance(UObject* WorldContextObject, UTinyRogueGameInstance*& OutGameInstance)
{
	if (IsValid(GEngine) == false)
		return false;
	
	const UWorld* World {GEngine->GetWorldFromContextObjectChecked(WorldContextObject)};
	if (IsValid(World) == false)
		return false;
	
	OutGameInstance = Cast<UTinyRogueGameInstance>(World->GetGameInstance());
	return IsValid(OutGameInstance);
}

bool UUtilityFunctionsLibrary::GetTinyRogueGameMode(UObject* WorldContextObject, ATinyRogueGameMode*& OutGameMode)
{
	if (IsValid(GEngine) == false)
		return false;
	
	const UWorld* World {GEngine->GetWorldFromContextObjectChecked(WorldContextObject)};
	if (IsValid(World) == false)
		return false;
	
	OutGameMode = Cast<ATinyRogueGameMode>(World->GetAuthGameMode());
	return IsValid(OutGameMode);
}

bool UUtilityFunctionsLibrary::GetCheatManager(UObject* WorldContextObject, UTinyRogueCheatManager*& OutCheatManager)
{
	if (IsValid(GEngine) == false)
		return false;
	
	const UWorld* World {GEngine->GetWorldFromContextObjectChecked(WorldContextObject)};
	if (IsValid(World) == false)
		return false;

	const APlayerController* PlayerController {World->GetFirstPlayerController()};
	if (IsValid(PlayerController) == false)
		return false;
	
	OutCheatManager = Cast<UTinyRogueCheatManager>(PlayerController->CheatManager);
	return IsValid(OutCheatManager);
}

FTransform UUtilityFunctionsLibrary::LerpTransform(const FTransform Start, const FTransform End, const float Alpha)
{
	const FVector NewLocation {FMath::Lerp(Start.GetLocation(), End.GetLocation(), Alpha)};
	const FQuat NewRotation {FQuat::Slerp(Start.GetRotation(), End.GetRotation(), Alpha)};
	const FVector NewScale {FMath::Lerp(Start.GetScale3D(), End.GetScale3D(), Alpha)};

	const FTransform ResultTransform(NewRotation, NewLocation, NewScale);
	return ResultTransform;
}

void UUtilityFunctionsLibrary::SetMaxFps(const float Value)
{
	IConsoleVariable* MaxFPSEditorVar {IConsoleManager::Get().FindConsoleVariable(TEXT("t.MaxFPS"))};
	MaxFPSEditorVar->Set(Value, ECVF_SetByConsole);
}

float UUtilityFunctionsLibrary::GetDefaultPhysicsStepDeltaTime()
{
	const IConsoleVariable* MaxFPSEditorVar {IConsoleManager::Get().FindConsoleVariable(TEXT("t.MaxFPS"))};
	float MaxFPSValue = MaxFPSEditorVar ? MaxFPSEditorVar->GetFloat() : -1.0f;

	if (MaxFPSValue <= 0.0f)
		MaxFPSValue = 120.0f;

	return 1.0f / MaxFPSValue;
}

void UUtilityFunctionsLibrary::DrawActorTransformedBoundingBox(const UObject* WorldContextObject, const FTransform Transform,
	const AActor* Actor, const FLinearColor Color, const float Duration, const float Thickness)
{
	if (IsValid(GEngine) == false || IsValid(Actor) == false)
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
