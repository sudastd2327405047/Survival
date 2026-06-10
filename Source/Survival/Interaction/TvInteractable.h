#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "TvInteractable.generated.h"

class APawn;

UCLASS()
class SURVIVAL_API ATvInteractable : public AInteractableActor
{
	GENERATED_BODY()

protected:
	virtual void OnInteractTriggered_Implementation(APawn* InstigatorPawn) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction|TV")
	void OnTvInteraction(APawn* InstigatorPawn);
};
