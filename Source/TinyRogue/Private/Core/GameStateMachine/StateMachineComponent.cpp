#include "Core/GameStateMachine/StateMachineComponent.h"

#include "Utility/UtilityFunctionsLibrary.h"
#include "Utility/DesignPatterns/StateMachine/StateInterface.h"

UStateMachineComponent::UStateMachineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStateMachineComponent::BeginPlay()
{
	Super::BeginPlay();
	
	TScriptInterface<IStateInterface> NewState;
	if (UUtilityFunctionsLibrary::CreateStateObject(GetOwner(), StartingStateClass, NewState))
		ChangeState_Implementation(NewState);
}

void UStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	Execute_Update(this, DeltaTime);
}

void UStateMachineComponent::ChangeState_Implementation(const TScriptInterface<IStateInterface>& NewState)
{
	if (IsValid(NewState.GetObject()) == false)
		return;
	
	if (IsValid(PreviousState.GetObject()))
		IStateInterface::Execute_Exit(PreviousState.GetObject());

	PreviousState = CurrentState;
	CurrentState = NewState;

	IStateInterface::Execute_AssignStateMachine(CurrentState.GetObject(), this);
	IStateInterface::Execute_Enter(CurrentState.GetObject());
}

void UStateMachineComponent::Update_Implementation(float DeltaTime)
{
	if (IsValid(CurrentState.GetObject()))
		IStateInterface::Execute_Update(CurrentState.GetObject(), DeltaTime);
}
