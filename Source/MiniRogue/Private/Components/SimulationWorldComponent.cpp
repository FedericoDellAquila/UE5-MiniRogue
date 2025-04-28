#include "Components/SimulationWorldComponent.h"

#include "Physics/Experimental/PhysScene_Chaos.h"
#include "Utility/Log.h"

USimulationWorldComponent::USimulationWorldComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

bool USimulationWorldComponent::CreateSimulationWorld()
{
	if (IsValid(SimulationWorld))
	{
		return true;
	}

	// Create a brand new world with a unique name and transient package
	const FName UniqueWorldName {MakeUniqueObjectName(nullptr, UWorld::StaticClass(), TEXT("SimulationWorld"))};

	const UWorld::InitializationValues InitValues {
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
		return false;
	}
	SimulationWorld->SetShouldTick(false);

	// Set world context
	FWorldContext& SimulationWorldContext {GEngine->CreateNewWorldContext(EWorldType::Game)};
	SimulationWorldContext.SetCurrentWorld(SimulationWorld);

	return IsValid(SimulationWorld);
}

void USimulationWorldComponent::CopyStaticActors(TArray<AActor*> InStaticActors)
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

void USimulationWorldComponent::CopyPhysicsActors(TArray<AActor*> InPhysicsActors)
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

void USimulationWorldComponent::SpawnPhysicsActors(TSoftClassPtr<AActor> ActorClass, TArray<FTransform> Transforms)
{
	if (IsValid(SimulationWorld) == false)
	{
		return;
	}

	if (ActorClass.IsNull())
	{
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

TArray<FPhysicsSimulationData> USimulationWorldComponent::PerformPhysicsSimulation(FPhysicsSimulationParameters PhysicsSimulationParameters, int32 MaxSteps/*= 300*/, bool bAutoDestroySimulationWorld/*= true*/)
{
	if (IsValid(SimulationWorld) == false || SimulationWorld->GetPhysicsScene() == nullptr)
	{
		return {};
	}

	if (MaxSteps <= 0)
	{
		LOG_WARNING("MaxSteps <= 0.")
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
		Actor->GetComponents(UPrimitiveComponent::StaticClass(), PhysicsSimulation.PrimitiveComponents, true);
		PhysicsSimulations.Emplace(PhysicsSimulation);
	}

	SetUpPhysicsSimulationFrame(PhysicsSimulationParameters);

	SimulationWorld->StartPhysicsSim();

	FPhysScene* PhysicsScene {SimulationWorld->GetPhysicsScene()};
	bool bAllAtRest {false};
	int32 StepCount = 0;
	while (!bAllAtRest && StepCount < MaxSteps)
	{
		PhysicsScene->StartFrame();
		PhysicsScene->WaitPhysScenes();

		for (FPhysicsSimulationData& TransformPhysicsSteps : PhysicsSimulations)
		{
			int32 Count {TransformPhysicsSteps.PrimitiveComponents.Num()};
			for (UPrimitiveComponent* MeshComp : TransformPhysicsSteps.PrimitiveComponents)
			{
				if (MeshComp->IsAnyRigidBodyAwake() == false)
				{
					Count--;
				}
			}
			if (Count == 0)
			{
				TransformPhysicsSteps.bIsAsleep = true;
			}
		}

		// Record transforms for this step
		int32 Sleepers {0};
		for (int32 i = 0; i < PhysicsSimulations.Num(); i++)
		{
			FPhysicsSimulationData& TransformPhysicsSteps = PhysicsSimulations[i];
			if (IsValid(TransformPhysicsSteps.Actor) && TransformPhysicsSteps.bIsAsleep == false)
			{
				TransformPhysicsSteps.Steps.Add(TransformPhysicsSteps.Actor->GetActorTransform());
			}
			else
			{
				Sleepers++;
				if (Sleepers == PhysicsSimulations.Num())
				{
					bAllAtRest = true;
				}
			}
		}

		PhysicsScene->EndFrame();
		StepCount++;
	}

	SimulationWorld->FinishPhysicsSim();

	if (bAutoDestroySimulationWorld)
	{
		DestroySimulationWorld();
	}

	return PhysicsSimulations;
}

bool USimulationWorldComponent::DestroySimulationWorld()
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

void USimulationWorldComponent::DuplicateActor(AActor* SourceActor, TArray<AActor*>& DestinationList) const
{
	if (IsValid(SimulationWorld) == false)
	{
		return;
	}

	if (IsValid(SourceActor) == false)
	{
		return;
	}

	UClass* SourceActorClass {SourceActor->GetClass()};

	FActorSpawnParameters SpawnParams;
	SpawnParams.Template = SourceActor;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* NewActor {SimulationWorld->SpawnActor<AActor>(SourceActorClass, SpawnParams)};
	DestinationList.Emplace(NewActor);
}

void USimulationWorldComponent::SetUpPhysicsSimulationFrame(FPhysicsSimulationParameters& PhysicsSimulationParameters) const
{
	if (IsValid(SimulationWorld) == false || SimulationWorld->GetPhysicsScene() == nullptr)
	{
		return;
	}

	if (PhysicsSimulationParameters.DeltaSeconds <= 0.0f)
	{
		PhysicsSimulationParameters.DeltaSeconds = 1.0f / 60.0f;
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
