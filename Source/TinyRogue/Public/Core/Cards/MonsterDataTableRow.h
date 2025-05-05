#pragma once

#include "MonsterDataTableRow.generated.h"

// TODO: divide this so that they can be accessed based on the level and separate monsters from boss.

UENUM(BlueprintType)
enum class EHealthType : uint8
{
	LevelBased,
	Fixed
};

USTRUCT(BlueprintType)
struct FBattleReward
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Experience;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Gold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bFinalTreasure;
};

USTRUCT(BlueprintType)
struct FMonsterDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString CreatureName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EHealthType HealthType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition="HealthType == EHealthType::Fixed", EditConditionHides))
	int32 Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Damage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBattleReward BattleReward;
};