#include "Components/SimulationWorldComponent.h"

#include "Physics/Experimental/PhysScene_Chaos.h"
#include "PhysicsEngine/PhysicsSettings.h"
#include "Utility/UtilityFunctionsLibrary.h"

USimulationWorldComponent::USimulationWorldComponent()
	: PhysScene(nullptr)
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
	UPackage* TransientPackage {GetTransientPackage()};
	const FName UniqueWorldName {MakeUniqueObjectName(nullptr, UWorld::StaticClass(), TEXT("SimulationWorld"))};

	const UWorld::InitializationValues InitValues {
		UWorld::InitializationValues()
		.AllowAudioPlayback(false)
		.RequiresHitProxies(false)
		.CreateNavigation(false)
		.CreateAISystem(false)
		.ShouldSimulatePhysics(true)
		.EnableTraceCollision(true)
		.CreatePhysicsScene(true)
	};

	SimulationWorld = UWorld::CreateWorld(EWorldType::Game, false, UniqueWorldName, TransientPackage, true, ERHIFeatureLevel::Num, &InitValues);
	if (IsValid(SimulationWorld) == false)
	{
		return false;
	}

	// Set world context
	FWorldContext& SimulationWorldContext {GEngine->CreateNewWorldContext(EWorldType::Game)};
	SimulationWorldContext.SetCurrentWorld(SimulationWorld);
	SimulationWorld->SetShouldTick(false);

	PhysScene = SimulationWorld->GetPhysicsScene();

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

void USimulationWorldComponent::PerformPhysicsSimulation(const FOnPhysicsSimulationTick& OnPhysicsSimulationTick, int32 MaxSteps/*= 300*/, float PhysicsStepDeltaTime/*= 0.01666666666f*/, bool bAutoDestroySimulationWorld/*= true*/)
{
	if (IsValid(SimulationWorld) == false || PhysScene == nullptr)
	{
		return;
	}

	if (PhysicsStepDeltaTime <= 0.0f)
	{
		PhysicsStepDeltaTime = UUtilityFunctionsLibrary::GetPhysicsStepDeltaTime();
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

	SimulationWorld->InitializeActorsForPlay(FURL()); // TODO: check if necessary

	SetPhysicsSimulationData(PhysicsStepDeltaTime);

	SimulationWorld->StartPhysicsSim();

	bool bAllAtRest {false};
	int32 StepCount = 0;

	while (!bAllAtRest && StepCount < MaxSteps)
	{
		PhysScene->StartFrame();

		PhysScene->WaitPhysScenes();

		if (StepCount > 0)
		{
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

		PhysScene->EndFrame();
		StepCount++;
	}

	SimulationWorld->FinishPhysicsSim();

	if (bAutoDestroySimulationWorld)
	{
		DestroySimulationWorld();
	}

	OnPhysicsSimulationTick.Execute(PhysicsSimulations);
}

bool USimulationWorldComponent::DestroySimulationWorld()
{
	if (IsValid(SimulationWorld))
	{
		GEngine->DestroyWorldContext(SimulationWorld);
		SimulationWorld->DestroyWorld(false);
		SimulationWorld = nullptr;
		PhysScene = nullptr;

		// Clear static and physics actors
		StaticActors.Empty();
		PhysicsActors.Empty();
	}
	return IsValid(SimulationWorld) == false;
}

void USimulationWorldComponent::CopyActorProperties(const AActor* SourceActor, AActor* TargetActor)
{
	if (!SourceActor || !TargetActor)
	{
		return;
	}

	for (TFieldIterator<FProperty> PropIt(SourceActor->GetClass()); PropIt; ++PropIt)
	{
		if (const FProperty* Property = *PropIt; !(Property->PropertyFlags & CPF_Transient))
		{
			const void* SourceValue = Property->ContainerPtrToValuePtr<void>(SourceActor);
			void* TargetValue = Property->ContainerPtrToValuePtr<void>(TargetActor);
			Property->CopyCompleteValue(TargetValue, SourceValue);
		}
	}
}

void USimulationWorldComponent::CopyPhysicsState(const AActor* SourceActor, const AActor* TargetActor)
{
	TArray<UPrimitiveComponent*> SourceComponents;
	SourceActor->GetComponents<UPrimitiveComponent>(SourceComponents);

	TArray<UPrimitiveComponent*> TargetComponents;
	TargetActor->GetComponents<UPrimitiveComponent>(TargetComponents);

	for (int32 i = 0; i < SourceComponents.Num() && i < TargetComponents.Num(); ++i)
	{
		UPrimitiveComponent* SourcePrim {SourceComponents[i]};
		UPrimitiveComponent* TargetPrim {TargetComponents[i]};

		if (SourcePrim->IsSimulatingPhysics())
		{
			TargetPrim->SetSimulatePhysics(SourcePrim->IsSimulatingPhysics());
			TargetPrim->SetPhysicsLinearVelocity(SourcePrim->GetPhysicsLinearVelocity());
			TargetPrim->SetPhysicsAngularVelocityInDegrees(SourcePrim->GetPhysicsAngularVelocityInDegrees());
			TargetPrim->SetWorldTransform(SourcePrim->GetComponentTransform());
		}
	}
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

	FActorSpawnParameters SpawnParams;
	SpawnParams.Template = SourceActor;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FTransform Transform {SourceActor->GetActorTransform()};
	UClass* ActorClass {SourceActor->GetClass()};

	AActor* NewActor {SimulationWorld->SpawnActor<AActor>(ActorClass, Transform, SpawnParams)};
	if (NewActor)
	{
		CopyActorProperties(SourceActor, NewActor);
		CopyPhysicsState(SourceActor, NewActor);
	}

	DestinationList.Emplace(NewActor);

	FVector Origin;
	FVector Bounds;
	NewActor->GetActorBounds(true, Origin, Bounds, false);

	UStaticMeshComponent* Prim = Cast<UStaticMeshComponent>(NewActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	Prim->GetLocalBounds(Origin, Bounds);

	DrawDebugBox(GetWorld(), Prim->GetComponentLocation(), Bounds - Origin, Prim->GetComponentRotation().Quaternion(), FColor::Green, true, 20.0f, 0, 10.0f);
}

void USimulationWorldComponent::SetPhysicsSimulationData(const float TimeStep) const
{
	if (IsValid(SimulationWorld) == false || PhysScene == nullptr)
	{
		return;
	}

	const FVector Gravity {FVector {0.0f, 0.0f, SimulationWorld->GetGravityZ()}};
	const UPhysicsSettings* PhysicsSettings = UPhysicsSettings::Get();

	PhysScene->SetUpForFrame(&Gravity, TimeStep,
		PhysicsSettings->MinPhysicsDeltaTime,
		PhysicsSettings->MaxPhysicsDeltaTime,
		PhysicsSettings->MaxSubstepDeltaTime,
		PhysicsSettings->MaxSubsteps,
		PhysicsSettings->bSubstepping);
}
