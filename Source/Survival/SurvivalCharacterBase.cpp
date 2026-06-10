#include "SurvivalCharacterBase.h"

#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Interaction/InteractableActor.h"
#include "EngineUtils.h"

ASurvivalCharacterBase::ASurvivalCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASurvivalCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerInputComponent && !InteractActionName.IsNone())
	{
		PlayerInputComponent->BindAction(InteractActionName, IE_Pressed, this, &ASurvivalCharacterBase::TryInteract);
	}
}

void ASurvivalCharacterBase::TryInteract()
{
	if (AInteractableActor* BestInteractable = FindBestInteractable())
	{
		BestInteractable->Interact(this);
	}
}

AInteractableActor* ASurvivalCharacterBase::FindBestInteractable() const
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}

	AInteractableActor* Best = nullptr;

	for (TActorIterator<AInteractableActor> It(World); It; ++It)
	{
		AInteractableActor* Candidate = *It;
		if (!IsValid(Candidate))
		{
			continue;
		}

		if (!Candidate->CanInteract(const_cast<ASurvivalCharacterBase*>(this)))
		{
			continue;
		}

		if (IsBetterCandidate(Candidate, Best, const_cast<ASurvivalCharacterBase*>(this)))
		{
			Best = Candidate;
		}
	}

	return Best;
}

bool ASurvivalCharacterBase::IsBetterCandidate(
	const AInteractableActor* Candidate,
	const AInteractableActor* CurrentBest,
	APawn* InteractingPawn) const
{
	if (!IsValid(Candidate))
	{
		return false;
	}

	if (!IsValid(CurrentBest))
	{
		return true;
	}

	const float CandidateDot = Candidate->GetFacingDotScore(InteractingPawn);
	const float BestDot = CurrentBest->GetFacingDotScore(InteractingPawn);
	if (!FMath::IsNearlyEqual(CandidateDot, BestDot))
	{
		return CandidateDot > BestDot;
	}

	const float CandidateDistanceSq = Candidate->GetDistanceSquaredToPawn(InteractingPawn);
	const float BestDistanceSq = CurrentBest->GetDistanceSquaredToPawn(InteractingPawn);
	return CandidateDistanceSq < BestDistanceSq;
}
