#pragma once
#include "CoreMinimal.h"
#include "Cards/DungeonLevelsDataTableRow.h"
#include "UObject/Object.h"
#include "DungeonLevelManagerComponent.generated.h"

USTRUCT(BlueprintType)
struct FDungeonLevel
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FDungeonLevelsDataTableRow LevelData;

	int32 StartIndex;	
	int32 EndIndex;
};

UCLASS()
class TINYROGUE_API UDungeonLevelManagerComponent : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
public:
	UDungeonLevelManagerComponent();

	UFUNCTION(BlueprintCallable)
	void Initialize();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 StartingRoom;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(RequiredAssetDataTags="RowStructure=/Script/TinyRogue.DungeonLevelsDataTableRow"))
	TObjectPtr<UDataTable> DungeonLevelsDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FDungeonLevel> DungeonLevelRanges;

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentRoom() const { return CurrentRoom; }

	UFUNCTION(BlueprintCallable)
	bool TryGetCurrentLevel(FDungeonLevel& Level) const;

	UFUNCTION(BlueprintCallable)
	bool TrySetCurrentRoom(int32 NewRoomIndex);

	UFUNCTION(BlueprintCallable)
	int32 GetMaxNumberOfRooms() const { return MaxNumberOfRooms; }
	
	UFUNCTION(BlueprintCallable)
	bool TryMoveToNextRoom(bool& bNewLevel);

	UFUNCTION(BlueprintCallable)
	bool TryDropToNextLevel();

	UFUNCTION(BlueprintCallable)
	bool TryGetCurrentDungeonLevelData(int32& LevelIndex, int32& RoomIndex, FDungeonLevel& LevelData) const;

	UFUNCTION(BlueprintCallable)
	bool TryMapCurrentRoomToLevelAndRelativeRoom(int32 GlobalIndex, int32& OutLevelIndex, int32& OutRoomInLevel) const;
	
private:
	int32 CurrentRoom;
	int32 MaxNumberOfRooms;

	void ClampRoomIndexValue(int& RoomIndex) const;
	void ClampLevelIndexValue(int& LevelIndex) const;
};
