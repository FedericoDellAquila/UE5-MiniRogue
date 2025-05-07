#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DungeonLevelManager.generated.h"

UCLASS()
class TINYROGUE_API UDungeonLevelManager : public UObject
{
	GENERATED_BODY()

public:
	UDungeonLevelManager();

private:
	int32 CurrentRoom;
};
