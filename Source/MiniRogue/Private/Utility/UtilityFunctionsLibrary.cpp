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

FRotator UUtilityFunctionsLibrary::RotateFaces(int32 Value)
{
	FVector Axis;

	switch (Value)
	{
		case 1:
		case 6:
			Axis = FVector::ForwardVector; // Flip around X to go from top to bottom
			break;
		case 2:
		case 5:
			Axis = FVector::RightVector; // Flip around Y to go from front to back
			break;
		case 3:
		case 4:
			Axis = FVector::UpVector; // Flip around Z to go from left to right
			break;
		default:
			Axis = FVector::ZeroVector;
			break;
	}

	FQuat FlipQuat = FQuat(Axis, PI); // 180 degrees
	return FlipQuat.Rotator();
}

void UUtilityFunctionsLibrary::DrawCubeMeshEdges(UStaticMeshComponent* MeshComp)
{
	if (!IsValid(MeshComp))
	{
		return;
	}

	// Cube vertices in local space assuming the mesh is 100x100x100 centered at origin
	TArray<FVector> LocalVertices = {
		FVector(-99, -99, -99) * MeshComp->GetOwner()->GetActorScale(),
		FVector(99, -99, -99) * MeshComp->GetOwner()->GetActorScale(),
		FVector(99, 99, -99) * MeshComp->GetOwner()->GetActorScale(),
		FVector(-99, 99, -99) * MeshComp->GetOwner()->GetActorScale(),
		FVector(-99, -99, 99) * MeshComp->GetOwner()->GetActorScale(),
		FVector(99, -99, 99) * MeshComp->GetOwner()->GetActorScale(),
		FVector(99, 99, 99) * MeshComp->GetOwner()->GetActorScale(),
		FVector(-99, 99, 99) * MeshComp->GetOwner()->GetActorScale(),
	};

	TArray<TPair<int32, int32>> Edges = {
		{0, 1}, {1, 2}, {2, 3}, {3, 0},
		{4, 5}, {5, 6}, {6, 7}, {7, 4},
		{0, 4}, {1, 5}, {2, 6}, {3, 7}
	};

	const FTransform& Transform = MeshComp->GetComponentTransform();

	for (const auto& Edge : Edges)
	{
		FVector StartWorld = Transform.TransformPosition(LocalVertices[Edge.Key]);
		FVector EndWorld = Transform.TransformPosition(LocalVertices[Edge.Value]);

		DrawDebugLine(MeshComp->GetWorld(), StartWorld, EndWorld, FColor::Green, false, 5.0f, 0, 1.0f);
	}
}

void UUtilityFunctionsLibrary::DrawBoxEdgesFromMeshComponent(UStaticMeshComponent* MeshComp, FColor Color, float Duration, float Thickness)
{
	if (!IsValid(MeshComp))
	{
		return;
	}

	FBox LocalBox = MeshComp->GetStaticMesh()->GetBoundingBox(); // Local-space AABB

	// Get 8 corners of the local bounding box
	FVector Min = LocalBox.Min;
	FVector Max = LocalBox.Max;

	TArray<FVector> LocalCorners;
	LocalCorners.Add(FVector(Min.X, Min.Y, Min.Z)); // 0
	LocalCorners.Add(FVector(Max.X, Min.Y, Min.Z)); // 1
	LocalCorners.Add(FVector(Max.X, Max.Y, Min.Z)); // 2
	LocalCorners.Add(FVector(Min.X, Max.Y, Min.Z)); // 3
	LocalCorners.Add(FVector(Min.X, Min.Y, Max.Z)); // 4
	LocalCorners.Add(FVector(Max.X, Min.Y, Max.Z)); // 5
	LocalCorners.Add(FVector(Max.X, Max.Y, Max.Z)); // 6
	LocalCorners.Add(FVector(Min.X, Max.Y, Max.Z)); // 7

	// Transform corners to world space
	const FTransform& WorldTransform = MeshComp->GetComponentTransform();
	for (FVector& Corner : LocalCorners)
	{
		Corner = WorldTransform.TransformPosition(Corner);
	}

	// Define the 12 edges between the corners
	const int EdgeIndices[12][2] = {
		{0, 1}, {1, 2}, {2, 3}, {3, 0}, // Bottom square
		{4, 5}, {5, 6}, {6, 7}, {7, 4}, // Top square
		{0, 4}, {1, 5}, {2, 6}, {3, 7} // Vertical edges
	};

	// Draw the lines
	UWorld* World = MeshComp->GetWorld();
	for (const auto& Edge : EdgeIndices)
	{
		DrawDebugLine(World, LocalCorners[Edge[0]], LocalCorners[Edge[1]], Color, false, Duration, 0, Thickness);
	}
}

void UUtilityFunctionsLibrary::DrawPlanes(UStaticMeshComponent* MeshComp, FColor Color, float Duration)
{
	// Assuming you have a UStaticMeshComponent* MeshComponent already
	UStaticMesh* StaticMesh = MeshComp->GetStaticMesh();
	FStaticMeshRenderData* RenderData = StaticMesh->GetRenderData();
	FPositionVertexBuffer& VertexBuffer = RenderData->LODResources[0].VertexBuffers.PositionVertexBuffer;

	TArray<FVector> Vertices;
	for (uint32 i = 0; i < VertexBuffer.GetNumVertices(); i++)
	{
		FVector3f Vec {VertexBuffer.VertexPosition(i)};
		Vertices.Add({Vec.X, Vec.Y, Vec.Z});
	}

	// Define the face normals
	FVector FaceNormals[6] = {
		FVector(0, 0, 1), // Front face
		FVector(0, 0, -1), // Back face
		FVector(1, 0, 0), // Right face
		FVector(-1, 0, 0), // Left face
		FVector(0, 1, 0), // Top face
		FVector(0, -1, 0), // Bottom face
	};

	// Define the face indices (each face has 4 vertices)
	TArray<TArray<int32>> FaceIndices = {
		{0, 1, 2, 3}, // Front
		{4, 5, 6, 7}, // Back
		{0, 1, 4, 5}, // Right
		{2, 3, 6, 7}, // Left
		{0, 3, 4, 7}, // Top
		{1, 2, 5, 6}, // Bottom
	};

	for (int i = 0; i < 6; i++)
	{
		// Get the 3 points of the face
		FVector A = Vertices[FaceIndices[i][0]];
		FVector B = Vertices[FaceIndices[i][1]];
		FVector C = Vertices[FaceIndices[i][2]];

		// Calculate the center of the face
		FVector FaceCenter = (A + B + C) / 3.0f;

		// Create the plane equation (point and normal)
		FPlane FacePlane(FaceCenter, FaceNormals[i]);

		// Define the extents of the plane (e.g., length and width)
		FVector2D PlaneExtents(100.0f, 100.0f); // Adjust these values as necessary

		// Draw the solid debug plane
		DrawDebugSolidPlane(MeshComp->GetOwner()->GetWorld(), FacePlane, FaceCenter, PlaneExtents, Color, false, Duration, 0);
	}
}
