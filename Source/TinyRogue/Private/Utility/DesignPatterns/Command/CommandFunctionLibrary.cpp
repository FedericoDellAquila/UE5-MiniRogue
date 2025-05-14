#include "Utility/DesignPatterns/Command/CommandFunctionLibrary.h"

#include "Utility/DesignPatterns/Command/CommandSubsystem.h"

bool UCommandFunctionLibrary::CommandEnqueue(UObject* WorldContextObject, UCommand* Command)
{
	UCommandSubsystem* CommandSubsystem {UCommandSubsystem::Get(WorldContextObject)};
	if (IsValid(CommandSubsystem) == false)
		return false;
	
	return CommandSubsystem->Enqueue(Command);
}

bool UCommandFunctionLibrary::CommandDequeue(UObject* WorldContextObject)
{	
	UCommandSubsystem* CommandSubsystem {UCommandSubsystem::Get(WorldContextObject)};
	if (IsValid(CommandSubsystem) == false)
		return false;
	
	return CommandSubsystem->Dequeue();
}
