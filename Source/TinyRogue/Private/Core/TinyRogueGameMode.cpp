#include "TinyRogue/Public/Core/TinyRogueGameMode.h"

#include "Core/GameStateMachine/GameStateMachine.h"
#include "Core/TimerTracker.h"
#include "Core/RollManager/RollManager.h"
#include "GameFramework/GameMode.h"
#include "Utility/Log.h"

// TODO: improve moving between rooms and levels

ATinyRogueGameMode::ATinyRogueGameMode()
	: LevelIndex(0)
	, RoomIndexInLevel(0)
	, MaxNumberOfRooms(0)
{}

void ATinyRogueGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	if (IsValid(RollManager) == false)
		RollManager = NewObject<URollManager>(this, RollManagerClass.LoadSynchronous());
	
	CLOG_FATAL(IsValid(RollManager) == false, "RollManager is nullptr.");

	if (IsValid(GameStateMachine) == false)
		GameStateMachine = NewObject<UGameStateMachine>(this, GameStateMachineClass.LoadSynchronous());
	
	CLOG_FATAL(IsValid(GameStateMachine) == false, "GameStateMachine is nullptr.");
	
	if (IsValid(TimeTracker) == false)
		TimeTracker = NewObject<UTimeTracker>(this);
	
	CLOG_FATAL(IsValid(TimeTracker) == false, "TimeTracker is nullptr.");
	
	for (const FDungeonLevel& Level : Dungeon.Levels)
		MaxNumberOfRooms += Level.NumberOfRooms;
	
	OnInitGame(MapName, Options, ErrorMessage);
}

FDungeonLevel ATinyRogueGameMode::GetCurrentDungeonLevel() const
{
	if (Dungeon.Levels.IsEmpty())
		return {};

	if (Dungeon.Levels.IsValidIndex(LevelIndex) == false)
		return {};

	return Dungeon.Levels[LevelIndex];
}

void ATinyRogueGameMode::SetRoomIndex(const int32 RoomIndex)
{
	if (RoomIndex >= MaxNumberOfRooms)
		return;
	
	LevelIndex = 0;
	
	int32 GlobalIndex {};
	for (int32 i = 0; i < Dungeon.Levels.Num(); ++i)
	{
		const int32 Rooms {Dungeon.Levels[i].NumberOfRooms};
		if (RoomIndex < GlobalIndex + Rooms)
			break;

		GlobalIndex += Rooms;
		LevelIndex++;
	}

	RoomIndexInLevel = RoomIndex - GlobalIndex;
}

void ATinyRogueGameMode::MoveToNextRoom()
{
	const FDungeonLevel& CurrentLevel {Dungeon.Levels[LevelIndex]};

	const int32 NextIndex {RoomIndexInLevel + 1};
	if (NextIndex >= MaxNumberOfRooms)
		return;
	
	if (RoomIndexInLevel + 1 < CurrentLevel.NumberOfRooms)
		RoomIndexInLevel++;
	else
		MoveToNextLevel();
}

void ATinyRogueGameMode::DropToNextLevel()
{
	if (Dungeon.Levels.IsValidIndex(LevelIndex + 1) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reached the final level!"));
		return;
	}
	LevelIndex++;
}

void ATinyRogueGameMode::MoveToNextLevel()
{
	if (Dungeon.Levels.IsValidIndex(LevelIndex + 1) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reached the final level!"));
		return;
	}
	LevelIndex++;
	RoomIndexInLevel = 0;
}

int32 ATinyRogueGameMode::GetCurrentRoom() const
{
	int32 GlobalIndex {};
	for (int32 i = 0; i < LevelIndex; ++i)
		GlobalIndex += Dungeon.Levels[i].NumberOfRooms;

	GlobalIndex += RoomIndexInLevel;
	return GlobalIndex;
}

void ATinyRogueGameMode::GetCurrentPosition(int32& RoomIndex, FDungeonLevel& DungeonLevel)
{
	RoomIndex = GetCurrentRoom();
	DungeonLevel = GetCurrentDungeonLevel();
}
