#include "Core/DungeonLevelManagerComponent.h"

#include "Core/Cards/DungeonLevelsDataTableRow.h"
#include "Utility/Log.h"

UDungeonLevelManagerComponent::UDungeonLevelManagerComponent()
	: StartingRoom(0)
	, CurrentRoom(0)
	, MaxNumberOfRooms(0)
{}

void UDungeonLevelManagerComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	Initialize();
}

void UDungeonLevelManagerComponent::Initialize()
{
	if (IsValid(DungeonLevelsDataTable) == false)
	{
		LOG_ERROR("DungeonLevelsDataTable is nullptr.");
		return;
	}

	TArray<FDungeonLevelsDataTableRow*> Rows {};
	DungeonLevelsDataTable->GetAllRows<FDungeonLevelsDataTableRow>({}, Rows);
	if (Rows.IsEmpty())
	{
		LOG_ERROR("DungeonLevelsDataTable is empty.");
		return;
	}

	DungeonLevelRanges.Empty();
	DungeonLevelRanges.Reserve(Rows.Num());
		
	int32 Rooms {0};
	for (const FDungeonLevelsDataTableRow* Row : Rows)
	{
		const FDungeonLevel Level
		{
			*Row,
			Rooms,
			Rooms + Row->Rooms - 1
		};
		DungeonLevelRanges.Emplace(Level);
		
		Rooms += Row->Rooms;
	}

	MaxNumberOfRooms = Rooms;

	StartingRoom = FMath::Clamp(StartingRoom, 0, MaxNumberOfRooms - 1);
	CurrentRoom = StartingRoom;
}

bool UDungeonLevelManagerComponent::TryGetCurrentLevel(FDungeonLevel& Level) const
{
	int32 LevelIndex {};
	int32 RoomIndex {};
	return TryGetCurrentDungeonLevelData(LevelIndex, RoomIndex, Level);
}

bool UDungeonLevelManagerComponent::TrySetCurrentRoom(int32 NewRoomIndex)
{
	ClampRoomIndexValue(NewRoomIndex);
	
	if (CurrentRoom != NewRoomIndex)
	{
		CurrentRoom = NewRoomIndex;
		return true;
	}
	return false;
}

bool UDungeonLevelManagerComponent::TryMoveToNextRoom(bool& bNewLevel)
{
	FDungeonLevel Level {};
	if (TryGetCurrentLevel(Level) == false)
	{
		LOG_ERROR("Current Dungeon Level is invalid.");
		return false;
	}

	const bool bResult {TrySetCurrentRoom(CurrentRoom + 1)};
	if (bResult == false)
		return false;

	FDungeonLevel NewLevel {};
	if (TryGetCurrentLevel(NewLevel) == false)
	{
		LOG_ERROR("Current Dungeon Level is invalid.");
		return false;
	}

	if (Level.LevelData.LevelName != NewLevel.LevelData.LevelName)
		bNewLevel = true;
	
	return true;
}

bool UDungeonLevelManagerComponent::TryDropToNextLevel()
{
	FDungeonLevel Level {};
	if (TryGetCurrentLevel(Level) == false)
	{
		LOG_ERROR("Current Dungeon Level is invalid.");
		return false;
	}
	
	int32 DropRoomIndex {CurrentRoom};
	const int32 RoomOffset {(Level.EndIndex - Level.StartIndex) + 1};
	if (DropRoomIndex + RoomOffset < MaxNumberOfRooms)
		DropRoomIndex += RoomOffset;

	if (DropRoomIndex == CurrentRoom)
		return false;

	CurrentRoom = DropRoomIndex;
	return true;
}

bool UDungeonLevelManagerComponent::TryGetCurrentDungeonLevelData(int32& LevelIndex, int32& RoomIndex, FDungeonLevel& LevelData) const
{
	const bool bResult {TryMapCurrentRoomToLevelAndRelativeRoom(CurrentRoom, LevelIndex, RoomIndex)};
	if (bResult == false)
	{
		LOG_ERROR("Current Dungeon Level is invalid.");
		return false;
	}

	LevelData = DungeonLevelRanges[LevelIndex];
	return true;
}

bool UDungeonLevelManagerComponent::TryMapCurrentRoomToLevelAndRelativeRoom(int32 GlobalIndex, int32& OutLevelIndex, int32& OutRoomInLevel) const
{
	ClampRoomIndexValue(GlobalIndex);

	OutRoomInLevel = GlobalIndex;
	for (int32 i = 0; i < DungeonLevelRanges.Num(); ++i)
	{
		const FDungeonLevel& Level {DungeonLevelRanges[i]};
		if (GlobalIndex >= Level.StartIndex && GlobalIndex <= Level.EndIndex)
		{
			OutLevelIndex = i;
			OutRoomInLevel -= Level.StartIndex;
			return true;
		}
	}

	OutLevelIndex = -1;
	OutRoomInLevel = -1;
	return false;
}

void UDungeonLevelManagerComponent::ClampRoomIndexValue(int& RoomIndex) const
{
	if (RoomIndex < 0)
	{
		RoomIndex = 0;
		LOG_WARNING("RoomIndex is a negative value. Setting RoomIndex to 0.");
	}
	else if (RoomIndex >= MaxNumberOfRooms)
	{
		RoomIndex = MaxNumberOfRooms - 1;
		LOG_WARNING("RoomIndex is greater than MaxNumberOfRooms. Setting RoomIndex to last index {0}.", RoomIndex);
	}
}

void UDungeonLevelManagerComponent::ClampLevelIndexValue(int& LevelIndex) const
{
	if (LevelIndex < 0)
	{
		LevelIndex = 0;
		LOG_WARNING("LevelIndex is a negative value. Setting LevelIndex to 0.");
	}
	else if (LevelIndex >= DungeonLevelRanges.Num())
	{
		LevelIndex = DungeonLevelRanges.Num() - 1;
		LOG_WARNING("RoomIndex is greater than MaxNumberOfRooms. Setting RoomIndex to last index {0}.", LevelIndex);
	}
}
