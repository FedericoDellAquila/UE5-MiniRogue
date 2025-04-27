#include "Utility/UtilityFunctionsLibrary.h"
#include "Core/GameplayGameMode.h"
#include "PhysicsEngine/PhysicsSettings.h"
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

float UUtilityFunctionsLibrary::GetPhysicsStepDeltaTime()
{
	const UPhysicsSettings* PhysicsSettings {UPhysicsSettings::Get()};
	return PhysicsSettings->MaxSubstepDeltaTime;
}

void UUtilityFunctionsLibrary::DrawBoxEdgesFromMeshComponent(UWorld* World, FTransform Transform, UStaticMeshComponent* MeshComp, FLinearColor Color, float Duration, float Thickness)
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
	for (const auto& Edge : EdgeIndices)
	{
		DrawDebugLine(World, LocalCorners[Edge[0]], LocalCorners[Edge[1]], Color.ToRGBE(), false, Duration, 0, Thickness);
	}
}
