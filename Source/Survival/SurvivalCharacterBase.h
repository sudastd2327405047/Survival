#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SurvivalCharacterBase.generated.h"

class AInteractableActor;
class APawn;

UCLASS()
class SURVIVAL_API ASurvivalCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ASurvivalCharacterBase();

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	FName InteractActionName = TEXT("Interact");

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void TryInteract();

	UFUNCTION(BlueprintPure, Category = "Interaction")
	AInteractableActor* FindBestInteractable() const;

	UFUNCTION(BlueprintPure, Category = "Interaction")
	bool IsBetterCandidate(
		const AInteractableActor* Candidate,
		const AInteractableActor* CurrentBest,
		APawn* InteractingPawn) const;
};
