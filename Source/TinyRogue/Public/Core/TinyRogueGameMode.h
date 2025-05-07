#pragma once
#include "CoreMinimal.h"
#include "Cards/DungeonLevelsDataTableRow.h"
#include "GameFramework/GameModeBase.h"
#include "TinyRogueGameMode.generated.h"

class UGameStateMachine;
class UTimeTracker;
class URollManager;

UCLASS()
class TINYROGUE_API ATinyRogueGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

public:
	ATinyRogueGameMode();
	
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<URollManager> RollManagerClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<URollManager> RollManager;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UGameStateMachine> GameStateMachineClass;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UGameStateMachine> GameStateMachine;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UTimeTracker> TimeTracker;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(RequiredAssetDataTags="RowStructure=/Script/TinyRogue.DungeonLevelsDataTableRow"))
	TObjectPtr<UDataTable> DungeonLevelsDataTable;
	
	UFUNCTION(BlueprintCallable)
	FDungeonLevelsDataTableRow GetCurrentDungeonLevel() const;

	UFUNCTION(BlueprintCallable)
	void SetRoomIndex(const int32 RoomIndex);
	
	UFUNCTION(BlueprintCallable)
	void MoveToNextRoom();

	UFUNCTION(BlueprintCallable)
	void DropToNextLevel();
	
	UFUNCTION(BlueprintCallable)
	void MoveToNextLevel();

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentRoom() const;

	UFUNCTION(BlueprintCallable)
	void GetCurrentPosition(int32& RoomIndex, FDungeonLevelsDataTableRow& DungeonLevel) const;

	UFUNCTION(BlueprintCallable)
	int32 GetMaxNumberOfRooms() const { return MaxNumberOfRooms; };
	
protected:
	UFUNCTION(BlueprintImplementableEvent, Category="TinyRogueGameMode")
	void OnInitGame(const FString& MapName, const FString& Options, const FString& ErrorMessage);
	
	int32 LevelIndex;
	int32 RoomIndexInLevel;
	int32 MaxNumberOfRooms;

private:
	TArray<FName> GetLevelsNames() const;
	const FDungeonLevelsDataTableRow* GetDungeonLevelData(const FName& DungeonLevelName) const;
	TArray<const FDungeonLevelsDataTableRow*> GetDungeonLevelsData() const;
};
