#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WorldObject.generated.h"

UCLASS(Blueprintable, BlueprintType, meta=(ShowWorldContextPin))
class TINYROGUE_API UWorldObject : public UObject
{
	GENERATED_BODY()
};
