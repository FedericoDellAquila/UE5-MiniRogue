#include "Utility/DesignPatterns/Command/CommandSubsystem.h"
#include "Utility/DesignPatterns/Command/Command.h"

UCommandSubsystem* UCommandSubsystem::Get(const UObject* WorldContextObject)
{
	const UWorld* World {GEngine->GetWorldFromContextObjectChecked(WorldContextObject)};
	return World->GetSubsystem<UCommandSubsystem>();
}

bool UCommandSubsystem::Enqueue(UCommand* Command)
{
	if (IsValid(Command) == false)
		return false;
	
	CommandQueue.Enqueue(Command);
	return true;
}

bool UCommandSubsystem::Dequeue()
{
	if (CommandQueue.IsEmpty())
		return false;
	
	TObjectPtr<UCommand> CommandPtr {nullptr};
	CommandQueue.Dequeue(CommandPtr);
	if (CommandPtr == nullptr)
		return false;

	UCommand* Command {CommandPtr.Get()};
	if (IsValid(Command) == false)
		return false;
	
	Command->MarkAsGarbage();
	Command->ConditionalBeginDestroy();
	return true;
}

void UCommandSubsystem::Tick(float DeltaTime)
{
	if (CommandQueue.IsEmpty())
		return;

	TObjectPtr<UCommand>* CommandPtr {CommandQueue.Peek()};
	if (CommandPtr == nullptr)
		return;

	UCommand* Command {*CommandPtr};
	if (IsValid(Command) == false)
		return;
	
	Command->Update(DeltaTime);
}

TStatId UCommandSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UCommandSubsystem, STATGROUP_Tickables);
}
