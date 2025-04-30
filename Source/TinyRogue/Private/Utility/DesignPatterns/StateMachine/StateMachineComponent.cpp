#include "Utility/DesignPatterns/StateMachine/StateMachineComponent.h"
#include "Utility/DesignPatterns/StateMachine/State.h"

UStateMachineComponent::UStateMachineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Execute_Update(this, DeltaTime);
}

void UStateMachineComponent::ChangeState_Implementation(const TScriptInterface<IState>& NewState)
{
	if (IsValid(PreviousState.GetObject()))
	{
		IState::Execute_Exit(PreviousState.GetObject());
	}

	PreviousState = CurrentState;
	CurrentState = NewState;

	IState::Execute_AssignStateMachine(CurrentState.GetObject(), this);

	IState::Execute_Enter(CurrentState.GetObject());
}

void UStateMachineComponent::Update_Implementation(float DeltaTime)
{
	if (IsValid(CurrentState.GetObject()))
	{
		IState::Execute_Update(CurrentState.GetObject(), DeltaTime);
	}
}
