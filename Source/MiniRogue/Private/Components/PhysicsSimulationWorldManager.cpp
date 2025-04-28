#include "Components/PhysicsSimulationWorldManager.h"
#include "Physics/Experimental/PhysScene_Chaos.h"
#include "Utility/Log.h"

UPhysicsWorldSimulationManager::UPhysicsWorldSimulationManager()
{}

bool UPhysicsWorldSimulationManager::CreateSimulationWorld()
{
	if (IsValid(SimulationWorld))
	{
		return true;
	}

	// Create a brand new world with a unique name and transient package
	const FName UniqueWorldName {MakeUniqueObjectName(this, UWorld::StaticClass(), TEXT("SimulationWorld"))};

	const UWorld::InitializationValues InitValues
	{
		UWorld::InitializationValues()
		.AllowAudioPlayback(false)
		.RequiresHitProxies(false)
		.CreateNavigation(false)
		.CreateAISystem(false)
		.ShouldSimulatePhysics(false)
		.EnableTraceCollision(true)
		.SetTransactional(false)
		.CreateFXSystem(false)
		.SetDefaultGameMode(nullptr)
	};

	SimulationWorld = UWorld::CreateWorld(EWorldType::Game, false, UniqueWorldName, GetTransientPackage(), true, ERHIFeatureLevel::Num, &InitValues);
	if (IsValid(SimulationWorld) == false)
	{
		LOG_ERROR("Failed to create simulation world.")
		return false;
	}
	SimulationWorld->SetShouldTick(false);

	// Set world context
	FWorldContext& SimulationWorldContext {GEngine->CreateNewWorldContext(EWorldType::Game)};
	SimulationWorldContext.SetCurrentWorld(SimulationWorld);

	return IsValid(SimulationWorld);
}

void UPhysicsWorldSimulationManager::CopyStaticActors(TArray<AActor*> InStaticActors)
{
	if (InStaticActors.Num() == 0)
	{
		return;
	}

	for (AActor* Actor : InStaticActors)
	{
		DuplicateActor(Actor, StaticActors);
	}
}

void UPhysicsWorldSimulationManager::CopyPhysicsActors(TArray<AActor*> InPhysicsActors)
{
	if (InPhysicsActors.Num() == 0)
	{
		return;
	}

	for (AActor* Actor : InPhysicsActors)
	{
		DuplicateActor(Actor, PhysicsActors);
	}
}

void UPhysicsWorldSimulationManager::SpawnPhysicsActors(TSoftClassPtr<AActor> ActorClass, TArray<FTransform> Transforms)
{
	if (IsValid(SimulationWorld) == false)
	{
		LOG_ERROR("SimulationWorld is nullptr.")
		return;
	}

	if (ActorClass.IsNull())
	{
		LOG_ERROR("ActorClass is nullptr.")
		return;
	}

	for (const FTransform& Transform : Transforms)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AActor* NewActor {SimulationWorld->SpawnActor<AActor>(ActorClass.LoadSynchronous(), Transform, SpawnParams)};
		PhysicsActors.Emplace(NewActor);
	}
}

TArray<FPhysicsSimulationData> UPhysicsWorldSimulationManager::PerformPhysicsSimulation(FPhysicsSimulationParameters PhysicsSimulationParameters, int32 MaxSteps/*= 300*/, bool bAutoDestroySimulationWorld/*= true*/)
{
	if (IsValid(SimulationWorld) == false || SimulationWorld->GetPhysicsScene() == nullptr)
	{
		LOG_ERROR("SimulationWorld is nullptr or PhysicsScene is nullptr.")
		return {};
	}

	if (MaxSteps <= 0)
	{
		LOG_ERROR("MaxSteps <= 0.")
		return {};
	}

	TArray<FPhysicsSimulationData> PhysicsSimulations;
	PhysicsSimulations.Reserve(PhysicsActors.Num());
	for (int32 i = 0; i < PhysicsActors.Num(); i++)
	{
		AActor*& Actor {PhysicsActors[i]};

		FPhysicsSimulationData PhysicsSimulation {};
		PhysicsSimulation.Actor = Actor;
		PhysicsSimulation.bIsAsleep = false;
		PhysicsSimulation.Id = i;
		Actor->GetComponents(UMeshComponent::StaticClass(), PhysicsSimulation.MeshComponents, true);
		PhysicsSimulations.Emplace(PhysicsSimulation);
	}

	SetUpPhysicsSimulationFrame(PhysicsSimulationParameters);


	FPhysScene* PhysicsScene {SimulationWorld->GetPhysicsScene()};
	bool bAllPhysicsActorsAtRest {false};
	int32 StepsCount {0};
	int32 Sleepers {0};

	SimulationWorld->StartPhysicsSim();
	while (bAllPhysicsActorsAtRest == false && StepsCount < MaxSteps)
	{
		PhysicsScene->StartFrame();
		PhysicsScene->WaitPhysScenes();

		for (FPhysicsSimulationData& TransformPhysicsSteps : PhysicsSimulations)
		{
			int32 Count {TransformPhysicsSteps.MeshComponents.Num()};
			for (UPrimitiveComponent* MeshComp : TransformPhysicsSteps.MeshComponents)
			{
				if (MeshComp->IsAnyRigidBodyAwake() == false)
				{
					Count--;
				}
			}
			if (Count == 0)
			{
				TransformPhysicsSteps.bIsAsleep = true;

				Sleepers++;
				if (Sleepers == PhysicsSimulations.Num())
				{
					bAllPhysicsActorsAtRest = true;
				}
			}
		}

		// Record transforms for this step
		for (FPhysicsSimulationData& PhysicsSimulation : PhysicsSimulations)
		{
			if (IsValid(PhysicsSimulation.Actor) && PhysicsSimulation.bIsAsleep == false)
			{
				PhysicsSimulation.Steps.Emplace(PhysicsSimulation.Actor->GetActorTransform());
			}
		}

		PhysicsScene->EndFrame();
		StepsCount++;
	}

	SimulationWorld->FinishPhysicsSim();

	if (bAutoDestroySimulationWorld)
	{
		DestroySimulationWorld();
	}

	return PhysicsSimulations;
}

bool UPhysicsWorldSimulationManager::DestroySimulationWorld()
{
	if (IsValid(SimulationWorld))
	{
		GEngine->DestroyWorldContext(SimulationWorld);
		SimulationWorld->RemoveFromRoot();
		SimulationWorld->DestroyWorld(false);
		SimulationWorld = nullptr;

		// Clear static and physics actors
		StaticActors.Empty();
		PhysicsActors.Empty();
	}
	return IsValid(SimulationWorld) == false;
}

void UPhysicsWorldSimulationManager::DuplicateActor(AActor* SourceActor, TArray<AActor*>& DestinationList) const
{
	if (IsValid(SimulationWorld) == false)
	{
		LOG_ERROR("SimulationWorld is nullptr.")
		return;
	}

	if (IsValid(SourceActor) == false)
	{
		LOG_ERROR("SourceActor is nullptr.")
		return;
	}

	UClass* SourceActorClass {SourceActor->GetClass()};

	FActorSpawnParameters SpawnParams;
	SpawnParams.Template = SourceActor;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* NewActor {SimulationWorld->SpawnActor<AActor>(SourceActorClass, SpawnParams)};
	DestinationList.Emplace(NewActor);
}

void UPhysicsWorldSimulationManager::SetUpPhysicsSimulationFrame(FPhysicsSimulationParameters& PhysicsSimulationParameters) const
{
	if (PhysicsSimulationParameters.DeltaSeconds <= 0.0f)
	{
		const FPhysicsSimulationParameters DefaultPhysicsSimulationParameters;
		PhysicsSimulationParameters.DeltaSeconds = DefaultPhysicsSimulationParameters.DeltaSeconds;
		LOG_WARNING("DeltaSeconds <= 0.0f. Fallback to default value ({0})", DefaultPhysicsSimulationParameters.DeltaSeconds);
	}

	SimulationWorld->GetPhysicsScene()->SetUpForFrame(
		&PhysicsSimulationParameters.Gravity,
		PhysicsSimulationParameters.DeltaSeconds,
		PhysicsSimulationParameters.MinPhysicsDeltaTime,
		PhysicsSimulationParameters.MaxPhysicsDeltaTime,
		PhysicsSimulationParameters.MaxSubstepDeltaTime,
		PhysicsSimulationParameters.MaxSubsteps,
		PhysicsSimulationParameters.bSubstepping);
}
