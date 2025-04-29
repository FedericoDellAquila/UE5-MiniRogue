#include "Utility/UtilityFunctionsLibrary.h"

#include "Log.h"
#include "Core/GameplayGameMode.h"
#include "Utility/MiniRogueCheatManager.h"

bool UUtilityFunctionsLibrary::GetGameplayGameMode(UObject* WorldContextObject, AGameplayGameMode*& OutGameMode)
{
	OutGameMode = Cast<AGameplayGameMode>(WorldContextObject->GetWorld()->GetAuthGameMode());
	return IsValid(OutGameMode);
}

bool UUtilityFunctionsLibrary::GetCheatManager(UObject* WorldContextObject, UMiniRogueCheatManager*& OutCheatManager)
{
	OutCheatManager = Cast<UMiniRogueCheatManager>(WorldContextObject->GetWorld()->GetFirstPlayerController()->CheatManager);
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
	{
		LOG_WARNING("MaxFPSValue <= 0. Fallback to 120.");
		MaxFPSValue = 120.0f;
	}

	return 1.0f / MaxFPSValue;
}

void UUtilityFunctionsLibrary::DrawOrientedBoundingBoxFromStaticMeshComponent(UObject* WorldContextObject, FTransform Transform,
	UStaticMeshComponent* MeshComp, FLinearColor Color, float Duration, float Thickness)
{
	if (IsValid(MeshComp) == false || IsValid(MeshComp->GetStaticMesh()) == false)
	{
		return;
	}

	const FBox LocalBox {MeshComp->GetStaticMesh()->GetBoundingBox()}; // Local-space AABB 

	// Get 8 corners of the local bounding box
	const FVector Min {LocalBox.Min};
	const FVector Max {LocalBox.Max};

	TArray<FVector> LocalCorners
	{
		FVector(Min.X, Min.Y, Min.Z), // 0
		FVector(Max.X, Min.Y, Min.Z), // 1
		FVector(Max.X, Max.Y, Min.Z), // 2
		FVector(Min.X, Max.Y, Min.Z), // 3
		FVector(Min.X, Min.Y, Max.Z), // 4
		FVector(Max.X, Min.Y, Max.Z), // 5
		FVector(Max.X, Max.Y, Max.Z), // 6
		FVector(Min.X, Max.Y, Max.Z) // 7
	};

	// Transform corners to world space
	for (FVector& Corner : LocalCorners)
	{
		Corner = Transform.TransformPosition(Corner);
	}

	// Define the 12 edges between the corners
	constexpr int EdgeIndices[12][2] {
		{0, 1}, {1, 2}, {2, 3}, {3, 0}, // Bottom square
		{4, 5}, {5, 6}, {6, 7}, {7, 4}, // Top square
		{0, 4}, {1, 5}, {2, 6}, {3, 7} // Vertical edges
	};

	// Draw the lines
	const UWorld* TargetWorld {GEngine->GetWorldFromContextObjectChecked(WorldContextObject)};
	for (const auto& Edge : EdgeIndices)
	{
		DrawDebugLine(TargetWorld, LocalCorners[Edge[0]], LocalCorners[Edge[1]], Color.ToRGBE(), false, Duration, 0, Thickness);
	}
}
