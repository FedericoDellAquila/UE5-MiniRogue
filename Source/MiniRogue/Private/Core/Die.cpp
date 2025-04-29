#include "Core/Die.h"
#include "Core/DieFace.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utility/Log.h"
#include "Utility/UtilityFunctionsLibrary.h"

ADie::ADie()
{
	PrimaryActorTick.bCanEverTick = true;

	FacesNormals =
	{
		{1, FVector::UpVector},
		{2, FVector::BackwardVector},
		{3, FVector::RightVector},
		{4, FVector::LeftVector},
		{5, FVector::ForwardVector},
		{6, FVector::DownVector},
	};

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	if (IsValid(MeshComponent))
	{
		SetRootComponent(MeshComponent);
		MeshComponent->SetSimulatePhysics(true);

		const ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshAsset {TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")};
		if (StaticMeshAsset.Succeeded())
		{
			MeshComponent->SetStaticMesh(StaticMeshAsset.Object);
		}

		const ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset {TEXT("Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'")};
		if (MaterialAsset.Succeeded())
		{
			MeshComponent->SetMaterial(0, MaterialAsset.Object);
		}
	}

	FacesRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("FacesRootComponent"));
	if (IsValid(FacesRootComponent))
		FacesRootComponent->SetupAttachment(MeshComponent);
}

void ADie::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	TArray<UDieFace*> FacesComponents {FindFaces()};
	for (int32 i = 0; i < FacesComponents.Num(); ++i)
	{
		const int32 FaceValue {i + 1};
		const FVector* NormalPtr {FacesNormals.Find(FaceValue)};
		if (NormalPtr == nullptr)
		{
			LOG_ERROR("Can't find Normal associated with value {0}", FaceValue)
			continue;
		}

		FVector Normal {*NormalPtr};
		UDieFace*& TextRender {FacesComponents[i]};
		TextRender->SetValue(FaceValue);
		TextRender->SetRelativeLocationAndRotation(Normal * 51.0f, Normal.Rotation());

#if WITH_EDITOR
		TextRender->Rename(*FString::Printf(TEXT("DieFace_%i"), FaceValue));
#endif
	}
}

void ADie::BeginPlay()
{
	Super::BeginPlay();

	DieFaces = FindFaces();
}

void ADie::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsReplicatingPhysicsSimulation)
		PhysicsSimulationStep(DeltaSeconds);
}

int32 ADie::GetValue() const
{
	int32 BestValue {0};
	double BestDotProduct {-INFINITY};
	for (const UDieFace* Face : DieFaces)
	{
		const double DotProduct {Face->GetFaceDirection().Dot(FVector::UpVector)};
		if (DotProduct > BestDotProduct)
		{
			BestDotProduct = DotProduct;
			BestValue = Face->GetValue();

			if (FMath::IsNearlyEqual(DotProduct, 1.0f))
				break;
		}
	}

	if (BestValue <= 0)
		LOG_ERROR("Failed to determine the value of the die.")

	return BestValue;
}

void ADie::RotateFromFaceToFace(int32 FromValue, int32 ToValue) const
{
	if (FromValue == ToValue)
		return;

	const FVector* FromNormalPtr {FacesNormals.Find(FromValue)};
	const FVector* ToNormalPtr {FacesNormals.Find(ToValue)};

	if (FromNormalPtr == nullptr)
	{
		LOG_ERROR("Can't find Normal value associated with {0}", FromValue)
		return;
	}

	if (ToNormalPtr == nullptr)
	{
		LOG_ERROR("Can't find Normal value associated with {0}", ToValue)
		return;
	}

	const FVector FromNormal {*FromNormalPtr};
	const FVector ToNormal {*ToNormalPtr};

	// Calculate rotation needed to rotate ToNormal to FromNormal
	const FQuat RotationQuat {FQuat::FindBetweenNormals(ToNormal, FromNormal)};

	// Reset rotation
	FacesRootComponent->SetRelativeRotation(FRotator::ZeroRotator);

	// Apply the rotation
	FacesRootComponent->AddRelativeRotation(RotationQuat);
}

TArray<UDieFace*> ADie::FindFaces() const
{
	TArray<USceneComponent*> ChildrenComponents {};
	FacesRootComponent->GetChildrenComponents(false, ChildrenComponents);

	TArray<UDieFace*> FoundDieFaces {};
	FoundDieFaces.Reserve(ChildrenComponents.Num());

	for (USceneComponent* ChildComponent : ChildrenComponents)
	{
		if (UDieFace* DieFace {Cast<UDieFace>(ChildComponent)}; IsValid(DieFace))
			FoundDieFaces.Emplace(DieFace);
	}

	return FoundDieFaces;
}

void ADie::PhysicsSimulationStep(const float DeltaSeconds)
{
	if (PhysicsSimulationData.Steps.IsValidIndex(PhysicsSimulationIndex) == false)
	{
		bIsReplicatingPhysicsSimulation = false;
		return;
	}

	ElapsedTime += DeltaSeconds;
	const float Alpha {FMath::Clamp(ElapsedTime / PhysicsSimulationData.DeltaTime, 0.0f, 1.0f)};

	const FTransform& TargetTransform {PhysicsSimulationData.Steps[PhysicsSimulationIndex]};
	const FTransform ResultTransform {UUtilityFunctionsLibrary::LerpTransform(StartingTransform, TargetTransform, Alpha)};

	SetActorTransform(ResultTransform);

	if (Alpha >= 1.0f)
	{
		StartingTransform = TargetTransform;
		PhysicsSimulationIndex++;
		ElapsedTime = 0.0f; // Reset for next interpolation
	}
}

UDieFace* ADie::GetFaceByValue(int32 Value) const
{
	TArray<USceneComponent*> ChildrenComponents {};
	FacesRootComponent->GetChildrenComponents(false, ChildrenComponents);

	for (USceneComponent* SceneComponent : ChildrenComponents)
	{
		UDieFace* DieFace {Cast<UDieFace>(SceneComponent)};
		if (IsValid(DieFace) == false)
			continue;

		const int32 FaceValue {DieFace->GetValue()};
		if (FaceValue != Value)
			continue;

		return DieFace;
	}

	return nullptr;
}

void ADie::ReproducePhysicsSimulation(const FPhysicsSimulationData& InPhysicsSimulationData)
{
	MeshComponent->SetSimulatePhysics(false);

	PhysicsSimulationData = InPhysicsSimulationData;
	PhysicsSimulationIndex = 1;

	SetActorTransform(PhysicsSimulationData.Steps[0]);
	StartingTransform = GetActorTransform();

	bIsReplicatingPhysicsSimulation = true;
}
