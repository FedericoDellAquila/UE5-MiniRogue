// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Utility/DesignPatterns/StateMachine/StateMachineInterface.h"
#include "StateMachineComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TINYROGUE_API UStateMachineComponent : public UActorComponent, public IStateMachineInterface
{
	GENERATED_BODY()
	
public:
	UStateMachineComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UObject> StartingStateClass;

	virtual void ChangeState_Implementation(const TScriptInterface<IStateInterface>& NewState) override;
	virtual TScriptInterface<IStateInterface> GetCurrentState_Implementation() const override { return CurrentState; };
	virtual TScriptInterface<IStateInterface> GetPreviousState_Implementation() const override { return PreviousState; };

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Update_Implementation(float DeltaTime) override;

private:
	UPROPERTY()
	TScriptInterface<IStateInterface> CurrentState;

	UPROPERTY()
	TScriptInterface<IStateInterface> PreviousState;
};
