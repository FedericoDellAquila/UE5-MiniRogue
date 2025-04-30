// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateMachine.h"
#include "Components/ActorComponent.h"
#include "StateMachineComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TINYROGUE_API UStateMachineComponent : public UActorComponent, public IStateMachine
{
	GENERATED_BODY()

public:
	UStateMachineComponent();

	virtual void ChangeState_Implementation(const TScriptInterface<IState>& NewState) override;
	virtual TScriptInterface<IState> GetCurrentState_Implementation() const override { return CurrentState; };
	virtual TScriptInterface<IState> GetPreviousState_Implementation() const override { return PreviousState; };

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Update_Implementation(float DeltaTime) override;

private:
	UPROPERTY()
	TScriptInterface<IState> CurrentState;

	UPROPERTY()
	TScriptInterface<IState> PreviousState;
};
