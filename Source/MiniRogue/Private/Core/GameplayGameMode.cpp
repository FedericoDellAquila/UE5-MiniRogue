#include "MiniRogue/Public/Core/GameplayGameMode.h"
#include "Physics/Experimental/PhysScene_Chaos.h"
#include "PhysicsEngine/BodyInstance.h"

void AGameplayGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	OnInitGame(MapName, Options, ErrorMessage);
}

APawn* AGameplayGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	return SpawnDefaultPawnAtTransform(NewPlayer, StartSpot->GetActorTransform());
}

void AGameplayGameMode::SimulateRoll(TArray<FDiceAnimationPath>& OutAnimationPaths, int32 NumDice)
{
	if (SimWorld)
	{
		GEngine->DestroyWorldContext(SimWorld);
		SimWorld->DestroyWorld(false);
		SimWorld = nullptr;
	}

	// Create a brand new world with a unique name and transient package
	UPackage* TransientPackage = GetTransientPackage();
	FName UniqueWorldName = MakeUniqueObjectName(nullptr, UWorld::StaticClass(), TEXT("SimWorld"));

	const UWorld::InitializationValues InitValues = UWorld::InitializationValues()
													.AllowAudioPlayback(false)
													.RequiresHitProxies(false)
													.CreateNavigation(false)
													.CreateAISystem(false)
													.ShouldSimulatePhysics(true)
													.EnableTraceCollision(true)
													.CreatePhysicsScene(true);

	SimWorld = UWorld::CreateWorld(EWorldType::Game, false, UniqueWorldName, TransientPackage, true, ERHIFeatureLevel::Num, &InitValues);
	SimWorld->TickGroup = TG_PostPhysics;


	float Grav = SimWorld->GetGravityZ();
	UE_LOG(LogTemp, Error, TEXT("Gravity: %f"), Grav);

	// Set world context
	FWorldContext& SimWorldContext = GEngine->CreateNewWorldContext(EWorldType::Game);
	SimWorldContext.SetCurrentWorld(SimWorld);

	// Rename the persistent level to avoid WorldSettings name collision
	SimWorld->PersistentLevel->Rename(*MakeUniqueObjectName(SimWorld, ULevel::StaticClass(), TEXT("SimLevel")).ToString(), SimWorld);

	SimWorld->bTickNewlySpawned = true;

	// SimWorld->bShouldSimulatePhysics = true;
	SimWorld->SetShouldTick(true);

	// Set up physics scene
	FPhysScene* PhysScene = SimWorld->GetPhysicsScene();
	if (!PhysScene)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get physics scene"));
		SimWorld->DestroyWorld(false);
		return;
	}

	PhysScene->StaticInit();

	// Create floor
	AActor* Floor = SimWorld->SpawnActor<AActor>(FloorActor.LoadSynchronous(), FVector(0.0f, 0.0f, 0.5f), FRotator {});
	UStaticMeshComponent* FloorMesh = Cast<UStaticMeshComponent>(Floor->GetComponentByClass(UStaticMeshComponent::StaticClass()));

	FloorMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	FloorMesh->SetCollisionProfileName(TEXT("BlockAll"));
	FloorMesh->SetWorldScale3D(FVector(10.0f, 10.0f, 0.1f));
	FloorMesh->SetWorldLocation(FVector(0, 0, -10.0f));
	FloorMesh->SetMobility(EComponentMobility::Type::Static);

	// Set up arrays to track dice actors
	TArray<AActor*> DiceActors;
	TArray<UStaticMeshComponent*> DiceMeshComponents;

	// Initialize the output animation paths array
	OutAnimationPaths.Empty(NumDice);
	for (int32 i = 0; i < NumDice; i++)
	{
		FDiceAnimationPath NewPath;
		NewPath.DieID = i;
		OutAnimationPaths.Add(NewPath);
	}

	// Create dice actors
	for (int32 i = 0; i < NumDice; i++)
	{
		// Create an actor for the die
		AActor* DiceActor = SimWorld->SpawnActor(DieClass.LoadSynchronous());
		if (!IsValid(DiceActor))
		{
			return;
		}

		Die = DiceActor;

		// Set initial position and apply forces
		FVector InitialPosition = FVector(
			FMath::RandRange(-100.0f, 100.0f),
			FMath::RandRange(-100.0f, 100.0f),
			100.0f + i * 20.0f
		);

		DiceActor->SetActorLocation(InitialPosition);

		UStaticMeshComponent* Mesh {Cast<UStaticMeshComponent>(DiceActor->GetComponentByClass(UStaticMeshComponent::StaticClass()))};
		Mesh->SetSimulatePhysics(true);
		Mesh->SetEnableGravity(true);
		Mesh->SetMobility(EComponentMobility::Movable);
		Mesh->WakeAllRigidBodies();

		// Add initial transform to animation path
		OutAnimationPaths[i].Transforms.Add(DiceActor->GetActorTransform());

		// Apply random impulse and torque
		Mesh->AddImpulse(FVector(
			FMath::RandRange(-100000.0f, 100000.0f),
			FMath::RandRange(-100000.0f, 100000.0f),
			FMath::RandRange(0.0f, 20000.0f)
		));

		Mesh->AddAngularImpulseInDegrees(FVector(
			FMath::RandRange(-1500000.0f, 1500000.0f),
			FMath::RandRange(-1500000.0f, 1500000.0f),
			FMath::RandRange(-1500000.0f, 1500000.0f)
		));


		DiceActors.Add(DiceActor);
		DiceMeshComponents.Add(Mesh);
	}

	// Manually step the simulation
	constexpr float TimeStep = 1.0f / 60.0f; // ~60 fps
	constexpr int32 MaxSteps = 100; // Safety limit
	int32 StepCount = 0;
	bool bAllAtRest = false;

	// Begin play and simulation
	SimWorld->InitializeActorsForPlay(FURL());
	SimWorld->BeginPlay();

	while (!bAllAtRest && StepCount < MaxSteps)
	{
		SimWorld->StartPhysicsSim();

		// Tick the world - this advances physics
		SimWorld->Tick(LEVELTICK_All, TimeStep);

		// DiceMeshComponents[0]->AddImpulse(FVector(
		// 	FMath::RandRange(-100000.0f, 100000.0f),
		// 	FMath::RandRange(-100000.0f, 100000.0f),
		// 	FMath::RandRange(0.0f, 20000.0f)
		// ));
		//
		// DiceMeshComponents[0]->AddAngularImpulseInDegrees(FVector(
		// 	FMath::RandRange(-1500000.0f, 1500000.0f),
		// 	FMath::RandRange(-1500000.0f, 1500000.0f),
		// 	FMath::RandRange(-1500000.0f, 1500000.0f)
		// ));

		// Record transforms for this step
		for (int32 i = 0; i < DiceActors.Num(); i++)
		{
			if (DiceActors[i] && DiceActors[i]->IsValidLowLevel())
			{
				OutAnimationPaths[i].Transforms.Add(DiceActors[i]->GetActorTransform());

				DrawDebugBox(GetWorld(), DiceActors[i]->GetActorTransform().GetLocation(), FVector(10.0f, 10.0f, 10.0f), FColor::Red, false, 20.0f, 0, 1);
			}
		}

		// Check if all dice are at rest
		// bAllAtRest = true;
		// for (UStaticMeshComponent* MeshComp : DiceMeshComponents)
		// {
		// 	if (MeshComp && MeshComp->IsValidLowLevel() && MeshComp->IsSimulatingPhysics())
		// 	{
		// 		constexpr float VelocityThreshold = 1.0f;
		// 		constexpr float AngularVelocityThreshold = 1.0f;
		//
		// 		if (MeshComp->GetPhysicsLinearVelocity().Size() > VelocityThreshold ||
		// 			MeshComp->GetPhysicsAngularVelocityInDegrees().Size() > AngularVelocityThreshold)
		// 		{
		// 			bAllAtRest = false;
		// 			break;
		// 		}
		// 	}
		// }

		// PhysScene->Flush();
		// SimWorld->SendAllEndOfFrameUpdates();

		SimWorld->UpdateWorldComponents(false, true);
		SimWorld->UpdateLevelStreaming();
		SimWorld->SendAllEndOfFrameUpdates();

		PhysScene->WaitPhysScenes();
		SimWorld->FinishPhysicsSim();
		StepCount++;
	}

	UE_LOG(LogTemp, Log, TEXT("Dice simulation completed in %d steps"), StepCount);

	// Clean up
	GEngine->DestroyWorldContext(SimWorld);
	SimWorld->DestroyWorld(false);
	SimWorld = nullptr;
}
