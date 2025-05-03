#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TinyRoguePlayerController.generated.h"

class UStateMachineComponent;

UCLASS()
class TINYROGUE_API ATinyRoguePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATinyRoguePlayerController();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UStateMachineComponent> InputStateMachine;
};
