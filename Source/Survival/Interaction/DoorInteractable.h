#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "DoorInteractable.generated.h"

class APawn;

UCLASS()
class SURVIVAL_API ADoorInteractable : public AInteractableActor
{
	GENERATED_BODY()

protected:
	virtual void OnInteractTriggered_Implementation(APawn* InstigatorPawn) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction|Door")
	void OnDoorInteraction(APawn* InstigatorPawn);
};
