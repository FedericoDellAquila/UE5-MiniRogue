#pragma once
#include "CoreMinimal.h"
#include "Components/PhysicsSimulationWorldManager.h"
#include "GameFramework/Actor.h"
#include "Die.generated.h"

class UDieFace;

// TODO: implement a Delegate to notify the system for when the physics replication is finished

UCLASS()
class TINYROGUE_API ADie : public AActor
{
	GENERATED_BODY()

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	ADie();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<int32, FVector> FacesNormals;

	UFUNCTION(BlueprintCallable, BlueprintPure="false")
	int32 GetValue() const;

	UFUNCTION(BlueprintCallable)
	void RotateFromFaceToFace(int32 FromValue, int32 ToValue) const;

	UFUNCTION(BlueprintCallable)
	const TArray<UDieFace*>& GetFaces() const { return DieFaces; };

	UFUNCTION(BlueprintCallable)
	UDieFace* GetFaceByValue(int32 Value) const;

	UFUNCTION(BlueprintCallable)
	void ReproducePhysicsSimulation(const FPhysicsSimulationData& InPhysicsSimulationData);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> FacesRootComponent;

private:
	UPROPERTY()
	TArray<UDieFace*> DieFaces;

	TArray<UDieFace*> FindFaces() const;

	bool bIsReplicatingPhysicsSimulation;
	float ElapsedTime;
	FTransform StartingTransform;
	int32 PhysicsSimulationIndex;
	FPhysicsSimulationData PhysicsSimulationData;

	void PhysicsSimulationStep(const float DeltaSeconds);
};
