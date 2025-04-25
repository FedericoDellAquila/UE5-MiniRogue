#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SimulationWorldComponent.generated.h"

USTRUCT(BlueprintType)
struct FPhysicsSimulationData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	AActor* Actor;

	UPROPERTY(BlueprintReadOnly)
	int32 Id;

	UPROPERTY(BlueprintReadOnly)
	bool bIsAsleep;

	UPROPERTY(BlueprintReadOnly)
	TArray<FTransform> Steps;

	UPROPERTY(BlueprintReadOnly)
	TArray<UPrimitiveComponent*> PrimitiveComponents;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnPhysicsSimulationTick, const TArray<FPhysicsSimulationData>&, PhysicsSimulation);

UCLASS(ClassGroup=(Custom))
class MINIROGUE_API USimulationWorldComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USimulationWorldComponent();

	UFUNCTION(BlueprintCallable, meta=(ReturnDisplayName="Success"))
	bool CreateSimulationWorld();

	UFUNCTION(BlueprintCallable)
	void CopyStaticActors(TArray<AActor*> InStaticActors);

	UFUNCTION(BlueprintCallable)
	void CopyPhysicsActors(TArray<AActor*> InPhysicsActors);

	UFUNCTION(BlueprintCallable)
	void SpawnPhysicsActors(TSoftClassPtr<AActor> ActorClass, TArray<FTransform> Transforms);

	UFUNCTION(BlueprintCallable, BlueprintPure="true")
	UWorld* GetSimulationWorld() const { return SimulationWorld; }

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetStaticActors() const { return StaticActors; }

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetPhysicsActors() const { return PhysicsActors; }

	UFUNCTION(BlueprintCallable)
	void PerformPhysicsSimulation(const FOnPhysicsSimulationTick& OnPhysicsSimulationTick, int32 MaxSteps = 300, float PhysicsStepDeltaTime = 0.01666666666f, bool bAutoDestroySimulationWorld = true);

	UFUNCTION(BlueprintCallable, meta=(ReturnDisplayName="Success"))
	bool DestroySimulationWorld();

private:
	UPROPERTY()
	TObjectPtr<UWorld> SimulationWorld;

	FPhysScene* PhysScene;

	UPROPERTY()
	TArray<AActor*> StaticActors;

	UPROPERTY()
	TArray<AActor*> PhysicsActors;

	static void CopyActorProperties(const AActor* SourceActor, AActor* TargetActor);
	static void CopyPhysicsState(const AActor* SourceActor, const AActor* TargetActor);
	void DuplicateActor(AActor* SourceActor, TArray<AActor*>& DestinationList) const;

	void SetPhysicsSimulationData(const float TimeStep) const;
};
