#include "Core/Cards/Card.h"
#include "Components/WidgetComponent.h"

ACard::ACard()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	SetRootComponent(DefaultSceneComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(DefaultSceneComponent);
	StaticMeshComponent->SetWorldScale3D({2.0f, 1.5f, 0.01f});
	if (const ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh {TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")}; StaticMesh.Succeeded())
		StaticMeshComponent->SetStaticMesh(StaticMesh.Object);

	if (const ConstructorHelpers::FObjectFinder<UMaterial> Material {TEXT("Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'")}; Material.Succeeded())
		StaticMeshComponent->SetMaterial(0, Material.Object);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(DefaultSceneComponent);
	WidgetComponent->SetRelativeRotation({90.0f,0.0f,180.0f});
	WidgetComponent->SetRelativeLocation({0.0f, 0.0f, 0.65f});
}
