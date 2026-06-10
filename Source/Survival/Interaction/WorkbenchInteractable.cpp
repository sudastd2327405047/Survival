#include "WorkbenchInteractable.h"

void AWorkbenchInteractable::OnInteractTriggered_Implementation(APawn* InstigatorPawn)
{
	Super::OnInteractTriggered_Implementation(InstigatorPawn);
	OnWorkbenchInteraction(InstigatorPawn);
}
