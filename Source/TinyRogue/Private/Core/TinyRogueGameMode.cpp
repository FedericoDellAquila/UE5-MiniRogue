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

	for (const FDungeonLevelsDataTableRow*& Level : GetDungeonLevelsData())
		MaxNumberOfRooms += Level->Rooms;
	
	OnInitGame(MapName, Options, ErrorMessage);
}

FDungeonLevelsDataTableRow ATinyRogueGameMode::GetCurrentDungeonLevel() const
{
	const TArray<const FDungeonLevelsDataTableRow*> LevelsData {GetDungeonLevelsData()};
	if (LevelsData.IsEmpty())
	{
		LOG_ERROR("LevelsData is empty.");
		return {};
	}
	
	if (LevelsData.IsValidIndex(LevelIndex) == false)
	{
		LOG_ERROR("Level {0} not found.", LevelIndex);
		return {};
	}

	return *LevelsData[LevelIndex];
}

void ATinyRogueGameMode::SetRoomIndex(const int32 RoomIndex)
{
	if (RoomIndex >= MaxNumberOfRooms)
		return;

	const TArray<const FDungeonLevelsDataTableRow*> LevelsData {GetDungeonLevelsData()};
	if (LevelsData.IsEmpty())
	{
		LOG_ERROR("LevelsData is empty.");
		return;
	}
	
	LevelIndex = 0;
	
	int32 GlobalIndex {};
	for (int32 i = 0; i < LevelsData.Num(); ++i)
	{
		const int32 Rooms {LevelsData[i]->Rooms};
		if (RoomIndex < GlobalIndex + Rooms)
			break;

		GlobalIndex += Rooms;
		LevelIndex++;
	}

	RoomIndexInLevel = RoomIndex - GlobalIndex;
}

void ATinyRogueGameMode::MoveToNextRoom()
{
	const FDungeonLevelsDataTableRow LevelData {GetCurrentDungeonLevel()};

	const int32 NextIndex {RoomIndexInLevel + 1};
	if (NextIndex >= MaxNumberOfRooms)
		return;
	
	if (RoomIndexInLevel + 1 < LevelData.Rooms)
		RoomIndexInLevel++;
	else
		MoveToNextLevel();
}

void ATinyRogueGameMode::DropToNextLevel()
{
	const TArray<const FDungeonLevelsDataTableRow*> LevelsData {GetDungeonLevelsData()};
	if (LevelsData.IsEmpty())
	{
		LOG_ERROR("LevelsData is empty.");
		return;
	}

	const int32 NextIndex {LevelIndex + 1};
	if (LevelsData.IsValidIndex(NextIndex) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reached the final level!"));
		return;
	}
	LevelIndex++;
}

void ATinyRogueGameMode::MoveToNextLevel()
{
	const TArray<const FDungeonLevelsDataTableRow*> LevelsData {GetDungeonLevelsData()};
	if (LevelsData.IsEmpty())
	{
		LOG_ERROR("LevelsData is empty.");
		return;
	}
	
	if (LevelsData.IsValidIndex(LevelIndex + 1) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reached the final level!"));
		return;
	}
	LevelIndex++;
	RoomIndexInLevel = 0;
}

int32 ATinyRogueGameMode::GetCurrentRoom() const
{
	const TArray<const FDungeonLevelsDataTableRow*> LevelsData {GetDungeonLevelsData()};
	if (LevelsData.IsEmpty())
	{
		LOG_ERROR("LevelsData is empty.");
		return -1;
	}
	
	int32 GlobalIndex {};
	for (int32 i = 0; i < LevelIndex; ++i)
		GlobalIndex += LevelsData[i]->Rooms;

	GlobalIndex += RoomIndexInLevel;
	return GlobalIndex;
}

TArray<FName> ATinyRogueGameMode::GetLevelsNames() const
{
	if (IsValid(DungeonLevelsDataTable) == false)
	{
		LOG_ERROR("DungeonLevelsDataTable is nullptr.");
		return {};
	}
	return DungeonLevelsDataTable->GetRowNames();
}

const FDungeonLevelsDataTableRow* ATinyRogueGameMode::GetDungeonLevelData(const FName& DungeonLevelName) const
{
	if (IsValid(DungeonLevelsDataTable) == false)
	{
		LOG_ERROR("DungeonLevelsDataTable is nullptr.");
		return {};
	}	
	return DungeonLevelsDataTable->FindRow<FDungeonLevelsDataTableRow>(DungeonLevelName, {});
}

TArray<const FDungeonLevelsDataTableRow*> ATinyRogueGameMode::GetDungeonLevelsData() const
{
	if (IsValid(DungeonLevelsDataTable) == false)
	{
		LOG_ERROR("DungeonLevelsDataTable is nullptr.");
		return {};
	}

	TArray<const FDungeonLevelsDataTableRow*> Rows {};
	DungeonLevelsDataTable->GetAllRows({}, Rows);	
	return Rows; 
}

void ATinyRogueGameMode::GetCurrentPosition(int32& RoomIndex, FDungeonLevelsDataTableRow& DungeonLevel) const
{
	RoomIndex = GetCurrentRoom();
	DungeonLevel = GetCurrentDungeonLevel();
}
