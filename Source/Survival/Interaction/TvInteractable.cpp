#include "TvInteractable.h"

void ATvInteractable::OnInteractTriggered_Implementation(APawn* InstigatorPawn)
{
	Super::OnInteractTriggered_Implementation(InstigatorPawn);
	OnTvInteraction(InstigatorPawn);
}
