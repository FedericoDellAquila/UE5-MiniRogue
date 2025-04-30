// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/DesignPatterns/Command/SpawnDiceCommand.h"

void USpawnDiceCommand::Execute_Implementation()
{
	SpawnDice();
}

void USpawnDiceCommand::Undo_Implementation()
{
	DestroyDice();
}

void USpawnDiceCommand::SpawnDice()
{
	DestroyDice();

	if (IsValid(DiceClass) == false || IsValid(WorldContextObject) == false || DiceCount <= 0)
	{
		return;
	}

	for (int32 i = 0; i < DiceCount; i++)
	{
		FTransform Transform {};
		AActor* Actor {WorldContextObject->GetWorld()->SpawnActor(DiceClass, &Transform, FActorSpawnParameters {})};
		DiceActors.Add(Actor);
	}
}

void USpawnDiceCommand::DestroyDice()
{
	for (AActor* Die : DiceActors)
	{
		Die->Destroy();
	}
}
