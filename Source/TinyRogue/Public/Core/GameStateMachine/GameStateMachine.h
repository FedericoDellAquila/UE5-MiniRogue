#pragma once
#include "CoreMinimal.h"
#include "Core/WorldObject.h"
#include "Utility/DesignPatterns/StateMachine/StateMachine.h"
#include "GameStateMachine.generated.h"

UCLASS(BlueprintType, Blueprintable)
class TINYROGUE_API UGameStateMachine : public UWorldObject, public IStateMachine
{
	GENERATED_BODY()

public:
	virtual void ChangeState_Implementation(const TScriptInterface<IState>& NewState) override;
};
