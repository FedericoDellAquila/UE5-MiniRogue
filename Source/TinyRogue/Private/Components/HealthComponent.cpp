#include "Components/HealthComponent.h"

#include "Utility/Log.h"

UHealthComponent::UHealthComponent()
	: MaxHealth(0.0f)
	, Health(0.0f)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthComponent::SetMaxHealth(const int32 MaxHealthValue)
{
	MaxHealth = MaxHealthValue;
}

void UHealthComponent::SetHealth(const int32 HealthValue)
{
	Health = HealthValue;
}

void UHealthComponent::Heal(const int32 HealValue)
{
	const int32 PreviousValue {Health};
	
	const int32 NewHealth {FMath::Min(Health + HealValue, MaxHealth)};
	Health = NewHealth;

	OnHealthChangedDelegate.Broadcast(this, PreviousValue, Health);
}

void UHealthComponent::Damage(const int32 DamageValue)
{
	const int32 PreviousValue {Health};
	
	const int32 NewHealth {FMath::Max(Health - DamageValue, 0)};
	Health = NewHealth;

	OnHealthChangedDelegate.Broadcast(this, PreviousValue, Health);

	if (Health == 0)
		OnDeathDelegate.Broadcast(this);
}

float UHealthComponent::GetNormalizedHealthValue() const
{
	return static_cast<float>(Health) / static_cast<float>(MaxHealth);
}
