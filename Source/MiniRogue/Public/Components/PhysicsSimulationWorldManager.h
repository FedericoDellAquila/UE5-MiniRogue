#pragma once
#include "CoreMinimal.h"
#include "PhysicsSimulationWorldManager.generated.h"

USTRUCT(BlueprintType)
struct FPhysicsSimulationParameters
{
	GENERATED_BODY()

	FPhysicsSimulationParameters();

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
	float DeltaTime;

	UPROPERTY(BlueprintReadOnly)
	TArray<FTransform> Steps;

	UPROPERTY(BlueprintReadOnly)
	TArray<UMeshComponent*> MeshComponents;

	UPROPERTY()
	bool bIsAsleep;
};

UCLASS(ClassGroup=(Custom))
class MINIROGUE_API UPhysicsWorldSimulationManager : public UObject
{
	GENERATED_BODY()

public:
	UPhysicsWorldSimulationManager();

	UFUNCTION(BlueprintCallable, meta=(ReturnDisplayName="Success"))
	bool CreateSimulationWorld();

	UFUNCTION(BlueprintCallable, meta=(ReturnDisplayName="Static Actors Clones"))
	TArray<AActor*> CopyStaticActors(TArray<AActor*> InStaticActors);

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> CopyPhysicsActors(TArray<AActor*> InPhysicsActors);

	UFUNCTION(BlueprintCallable, meta=(ReturnDisplayName="Spawned Physics Actors"))
	TArray<AActor*> SpawnPhysicsActors(TSoftClassPtr<AActor> ActorClass, TArray<FTransform> Transforms);

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

	void DuplicateActor(AActor* SourceActor, TArray<AActor*>& DestinationList, const FName& FunctionName = NAME_None);

	void SetUpPhysicsSimulationFrame(FPhysicsSimulationParameters& PhysicsSimulationParameters) const;

	UFUNCTION()
	void OnStaticActorDestroyed(AActor* StaticActor);

	UFUNCTION()
	void OnPhysicsActorDestroyed(AActor* PhysicsActor);
};
