#include "Utility/DesignPatterns/Command/CommandSubsystem.h"
#include "Utility/DesignPatterns/Command/Command.h"

UCommandSubsystem* UCommandSubsystem::Get(const UObject* WorldContextObject)
{
	if (IsValid(WorldContextObject) == false)
		return nullptr;

	if (IsValid(GEngine) == false)
		return nullptr;
	
	const UWorld* World {GEngine->GetWorldFromContextObjectChecked(WorldContextObject)};
	if (IsValid(World) == false)
		return nullptr;
	
	return World->GetSubsystem<UCommandSubsystem>();
}

bool UCommandSubsystem::Enqueue(UCommand* Command)
{
	if (IsValid(Command) == false)
		return false;
	
	return CommandQueue.Enqueue(Command);
}

bool UCommandSubsystem::Dequeue()
{
	if (CommandQueue.IsEmpty() || IsValid(CurrentCommand))
		return false;
	
	if (CommandQueue.Dequeue(CurrentCommand) == false)
		return false;

	CurrentCommand->OnFinished.AddUniqueDynamic(this, &UCommandSubsystem::DestroyCommand);
	CurrentCommand->Execute();
	return true;
}

void UCommandSubsystem::ConsumeCommands()
{
	if (CommandQueue.IsEmpty())
		return;

	Dequeue();
}

void UCommandSubsystem::DestroyCommand(UCommand*)
{
	if (IsValid(CurrentCommand) == false)
		return;
	
	CurrentCommand->RemoveFromRoot();
	CurrentCommand->MarkAsGarbage();
	CurrentCommand->ConditionalBeginDestroy();
	CurrentCommand = nullptr;

	ConsumeCommands();
}

bool UCommandSubsystem::IsTickable() const
{
	return IsValid(CurrentCommand);
}

void UCommandSubsystem::Tick(float DeltaTime)
{
	CurrentCommand->Update(DeltaTime);
}

TStatId UCommandSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UCommandSubsystem, STATGROUP_Tickables);
}
