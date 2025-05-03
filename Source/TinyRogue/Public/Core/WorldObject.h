#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WorldObject.generated.h"

// TODO: experiment with this. Instead of using UObject, use this to allow UObject to access nodes that require the WorldContext Pin.

UCLASS(Blueprintable, BlueprintType, meta=(ShowWorldContextPin))
class TINYROGUE_API UWorldObject : public UObject
{
	GENERATED_BODY()
};
