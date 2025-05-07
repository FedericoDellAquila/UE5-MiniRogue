#include "Core/Cards/MonsterCard.h"
#include "Components/HealthComponent.h"

AMonsterCard::AMonsterCard()
	: EnemyData{}
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}
