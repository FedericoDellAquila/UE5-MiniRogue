// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "DieFace.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MINIROGUE_API UDieFace : public UTextRenderComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDieFace();

	UFUNCTION(BlueprintCallable)
	int32 GetValue() const { return Value; }

	UFUNCTION(BlueprintCallable)
	void SetValue(int32 InValue);

	UFUNCTION(BlueprintCallable)
	FVector GetFaceDirection() const;

private:
	UPROPERTY(VisibleAnywhere, Category="MiniRogue|DieFace")
	int32 Value;
};
