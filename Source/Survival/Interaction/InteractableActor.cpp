#include "InteractableActor.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Pawn.h"

AInteractableActor::AInteractableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(SceneRoot);

	InteractionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionVolume"));
	InteractionVolume->SetupAttachment(SceneRoot);
	InteractionVolume->SetSphereRadius(250.0f);
	InteractionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionVolume->SetCollisionObjectType(ECC_WorldDynamic);
	InteractionVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActor::HandleVolumeBeginOverlap);
	InteractionVolume->OnComponentEndOverlap.AddDynamic(this, &AInteractableActor::HandleVolumeEndOverlap);
}

void AInteractableActor::Interact(APawn* InstigatorPawn)
{
	if (!CanInteract(InstigatorPawn))
	{
		return;
	}

	OnInteractTriggered(InstigatorPawn);
}

bool AInteractableActor::CanInteract(APawn* InstigatorPawn) const
{
	if (!IsValid(InstigatorPawn))
	{
		return false;
	}

	if (!IsPawnOverlapping(InstigatorPawn))
	{
		return false;
	}

	if (!bRequireFacing)
	{
		return true;
	}

	return GetFacingDotScore(InstigatorPawn) >= FacingDotThreshold;
}

bool AInteractableActor::IsPawnOverlapping(APawn* InstigatorPawn) const
{
	return IsValid(InstigatorPawn) && OverlappingPawns.Contains(InstigatorPawn);
}

float AInteractableActor::GetFacingDotScore(APawn* InstigatorPawn) const
{
	if (!IsValid(InstigatorPawn))
	{
		return -1.0f;
	}

	const FVector PawnForward = InstigatorPawn->GetActorForwardVector().GetSafeNormal();
	const FVector ToInteractable = (GetActorLocation() - InstigatorPawn->GetActorLocation()).GetSafeNormal();
	return FVector::DotProduct(PawnForward, ToInteractable);
}

float AInteractableActor::GetDistanceSquaredToPawn(APawn* InstigatorPawn) const
{
	if (!IsValid(InstigatorPawn))
	{
		return TNumericLimits<float>::Max();
	}

	return FVector::DistSquared(InstigatorPawn->GetActorLocation(), GetActorLocation());
}

void AInteractableActor::OnInteractTriggered_Implementation(APawn* InstigatorPawn)
{
}

void AInteractableActor::HandleVolumeBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (APawn* Pawn = Cast<APawn>(OtherActor))
	{
		OverlappingPawns.Add(Pawn);
	}
}

void AInteractableActor::HandleVolumeEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (APawn* Pawn = Cast<APawn>(OtherActor))
	{
		OverlappingPawns.Remove(Pawn);
	}
}
