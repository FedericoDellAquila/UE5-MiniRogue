#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChangedDelegate, UHealthComponent*, HealthComponent, float, PreviousHealth, float, CurrentHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathDelegate, UHealthComponent*, HealthComponent);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TINYROGUE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnHealthChangedDelegate OnHealthChangedDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDeathDelegate OnDeathDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Health;

	UFUNCTION(BlueprintCallable)
	void SetMaxHealth(const int32 MaxHealthValue);

	UFUNCTION(BlueprintCallable)
	void SetHealth(const int32 HealthValue);

	UFUNCTION(BlueprintCallable)
	void Heal(const int32 HealValue);

	UFUNCTION(BlueprintCallable)
	void Damage(const int32 DamageValue);
	
	UFUNCTION(BlueprintCallable)
	float GetNormalizedHealthValue() const;
};
