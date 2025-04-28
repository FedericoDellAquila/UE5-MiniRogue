#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SimulationWorldComponent.generated.h"

USTRUCT(BlueprintType)
struct FPhysicsSimulationParameters
{
	GENERATED_BODY()

	FPhysicsSimulationParameters()
		: Gravity(FVector(0.0f, 0.0f, -980.0f))
		, DeltaSeconds(1.0f / 60.0f)
		, MinPhysicsDeltaTime(0)
		, MaxPhysicsDeltaTime(1.0f / 30.0f)
		, MaxSubstepDeltaTime(1.0f / 60.0f)
		, MaxSubsteps(6)
		, bSubstepping(false) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Gravity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DeltaSeconds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinPhysicsDeltaTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxPhysicsDeltaTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSubstepDeltaTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxSubsteps;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSubstepping;
};

USTRUCT(BlueprintType)
struct FPhysicsSimulationData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	AActor* Actor;

	UPROPERTY(BlueprintReadOnly)
	int32 Id;

	UPROPERTY(BlueprintReadOnly)
	TArray<FTransform> Steps;

	UPROPERTY(BlueprintReadOnly)
	TArray<UPrimitiveComponent*> PrimitiveComponents;

	UPROPERTY()
	bool bIsAsleep;
};

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
	TArray<FPhysicsSimulationData> PerformPhysicsSimulation(FPhysicsSimulationParameters PhysicsSimulationParameters, int32 MaxSteps = 500, bool bAutoDestroySimulationWorld = true);

	UFUNCTION(BlueprintCallable, meta=(ReturnDisplayName="Success"))
	bool DestroySimulationWorld();

private:
	UPROPERTY()
	TObjectPtr<UWorld> SimulationWorld;

	UPROPERTY()
	TArray<AActor*> StaticActors;

	UPROPERTY()
	TArray<AActor*> PhysicsActors;

	void DuplicateActor(AActor* SourceActor, TArray<AActor*>& DestinationList) const;

	void SetUpPhysicsSimulationFrame(FPhysicsSimulationParameters& PhysicsSimulationParameters) const;
};
