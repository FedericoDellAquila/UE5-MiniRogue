#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TinyRogueGameMode.generated.h"

class UGameStateMachine;
class UTimeTracker;
class URollManager;

USTRUCT(BlueprintType)
struct FDungeonLevel
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString LevelName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NumberOfRooms;
};

USTRUCT(BlueprintType)
struct FDungeon
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FDungeonLevel> Levels;
};

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDungeon Dungeon;

	UFUNCTION(BlueprintCallable)
	FDungeonLevel GetCurrentDungeonLevel() const;

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
	void GetCurrentPosition(int32& RoomIndex, FDungeonLevel& DungeonLevel);

	UFUNCTION(BlueprintCallable)
	int32 GetMaxNumberOfRooms() const { return MaxNumberOfRooms; };
	
protected:
	UFUNCTION(BlueprintImplementableEvent, Category="TinyRogueGameMode")
	void OnInitGame(const FString& MapName, const FString& Options, const FString& ErrorMessage);

	int32 LevelIndex;
	int32 RoomIndexInLevel;
	int32 MaxNumberOfRooms;
};
