#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Survival/Inventory/StashTypes.h"
#include "SurvivalStashSaveGame.generated.h"

UCLASS()
class SURVIVAL_API USurvivalStashSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stash")
	TArray<FStashPageData> SavedPages;
};
