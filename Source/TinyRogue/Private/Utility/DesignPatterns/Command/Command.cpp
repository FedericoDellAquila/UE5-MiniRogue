#include "Utility/DesignPatterns/Command/Command.h"
#include "Utility/DesignPatterns/Command/CommandSubsystem.h"

UCommand::UCommand()
	: bIsBeingExecuted(false)
{}

void UCommand::Execute_Implementation()
{
	bIsBeingExecuted = true;
}

void UCommand::Finish_Implementation()
{
	bIsBeingExecuted = false;
	// Notify UCommandSubsystem
}

void UCommand::Undo_Implementation()
{
	bIsBeingExecuted = false;
}

bool UCommand::IsBeingExecuted_Implementation() const
{
	return bIsBeingExecuted;
}

UCommandSubsystem* UCommand::GetCommandSubsystem_Implementation() const
{
	return UCommandSubsystem::Get(this);
}