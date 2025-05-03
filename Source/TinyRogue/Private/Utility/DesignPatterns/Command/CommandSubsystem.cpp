#include "Utility/DesignPatterns/Command/CommandSubsystem.h"
#include "Utility/DesignPatterns/Command/Command.h"

UCommandSubsystem* UCommandSubsystem::Get(const UObject* WorldContextObject)
{
	const UWorld* World {GEngine->GetWorldFromContextObjectChecked(WorldContextObject)};
	return World->GetGameInstance()->GetSubsystem<UCommandSubsystem>();
}

bool UCommandSubsystem::Enqueue(UCommand* Command, UCommand*& OutCommand)
{
	if (IsValid(Command) == false)
		return false;
	
	CommandQueue.Enqueue(Command);
	OutCommand = Command;
	return true;
}

bool UCommandSubsystem::Dequeue(UCommand*& OutCommand)
{
	if (CommandQueue.IsEmpty())
		return false;
	
	CommandQueue.Dequeue(OutCommand);
	return true;
}
