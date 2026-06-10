#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

class APawn;
class UPrimitiveComponent;
class USphereComponent;
class UStaticMeshComponent;
class USceneComponent;
struct FHitResult;

UCLASS(Abstract)
class SURVIVAL_API AInteractableActor : public AActor
{
	GENERATED_BODY()

public:
	AInteractableActor();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void Interact(APawn* InstigatorPawn);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool CanInteract(APawn* InstigatorPawn) const;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool IsPawnOverlapping(APawn* InstigatorPawn) const;

	UFUNCTION(BlueprintPure, Category = "Interaction")
	float GetFacingDotScore(APawn* InstigatorPawn) const;

	UFUNCTION(BlueprintPure, Category = "Interaction")
	float GetDistanceSquaredToPawn(APawn* InstigatorPawn) const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<USphereComponent> InteractionVolume;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool bRequireFacing = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (ClampMin = "-1.0", ClampMax = "1.0"))
	float FacingDotThreshold = 0.35f;

	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void OnInteractTriggered(APawn* InstigatorPawn);

private:
	UPROPERTY()
	TSet<TObjectPtr<APawn>> OverlappingPawns;

	UFUNCTION()
	void HandleVolumeBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void HandleVolumeEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
};
