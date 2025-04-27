// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Die.generated.h"

class UDieFace;
// class UTextRenderComponent;

UCLASS()
class MINIROGUE_API ADie : public AActor
{
	GENERATED_BODY()

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> FacesRootComponent;

private:
	UPROPERTY()
	TArray<UDieFace*> DieFaces;

	TArray<UDieFace*> FindFaces() const;
};
