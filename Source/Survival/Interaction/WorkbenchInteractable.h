#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "WorkbenchInteractable.generated.h"

class APawn;

UCLASS()
class SURVIVAL_API AWorkbenchInteractable : public AInteractableActor
{
	GENERATED_BODY()

protected:
	virtual void OnInteractTriggered_Implementation(APawn* InstigatorPawn) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction|Workbench")
	void OnWorkbenchInteraction(APawn* InstigatorPawn);
};
