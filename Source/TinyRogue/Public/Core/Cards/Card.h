#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Card.generated.h"

class UWidgetComponent;

UCLASS()
class TINYROGUE_API ACard : public AActor
{
	GENERATED_BODY()
	
public:
	ACard();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> DefaultSceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> WidgetComponent;
};
