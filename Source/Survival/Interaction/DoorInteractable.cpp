#include "DoorInteractable.h"

void ADoorInteractable::OnInteractTriggered_Implementation(APawn* InstigatorPawn)
{
	Super::OnInteractTriggered_Implementation(InstigatorPawn);
	OnDoorInteraction(InstigatorPawn);
}
