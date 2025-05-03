// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameStateMachine/GameStateMachine.h"

#include "Utility/Log.h"

void UGameStateMachine::ChangeState_Implementation(const TScriptInterface<IState>& NewState)
{
	IStateMachine::ChangeState_Implementation(NewState);

	LOG_WARNING("{0}", NewState.GetObject()->GetName())
}
