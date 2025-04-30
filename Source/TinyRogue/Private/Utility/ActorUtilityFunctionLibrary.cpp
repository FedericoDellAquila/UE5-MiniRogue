#include "Utility/ActorUtilityFunctionLibrary.h"

#include "Log.h"

UWorld* UActorUtilityFunctionLibrary::GetCurrentWorld(UObject* WorldContextObject)
{
	if (IsValid(GEngine) == false)
	{
		LOG_ERROR("GEngine is nullptr.")
		return nullptr;
	}

	return GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
}

AActor* UActorUtilityFunctionLibrary::CloneActorToWorld(AActor* SourceActor, UWorld* TargetWorld)
{
	if (IsValid(SourceActor) == false)
	{
		return nullptr;
	}

	if (IsValid(TargetWorld) == false)
	{
		TargetWorld = GetCurrentWorld(SourceActor);
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Template = SourceActor;
	SpawnParams.Owner = SourceActor->GetOwner();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Name = MakeUniqueObjectName(TargetWorld, SourceActor->GetClass(), NAME_None, EUniqueObjectNameOptions::GloballyUnique);
	return TargetWorld->SpawnActor<AActor>(SourceActor->GetClass(), SpawnParams);
}
