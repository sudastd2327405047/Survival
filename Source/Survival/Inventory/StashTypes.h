#pragma once

#include "CoreMinimal.h"
#include "StashTypes.generated.h"

USTRUCT(BlueprintType)
struct SURVIVAL_API FStashGridConfig
{
	GENERATED_BODY()

	static constexpr int32 PageCount = 5;
	static constexpr int32 Rows = 9;
	static constexpr int32 Cols = 12;
	static constexpr int32 SlotsPerPage = Rows * Cols;
};

USTRUCT(BlueprintType)
struct SURVIVAL_API FStashCellRef
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stash")
	int32 Page = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stash")
	int32 Row = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stash")
	int32 Col = 0;

	FORCEINLINE bool IsValid() const
	{
		return Page >= 0 && Page < FStashGridConfig::PageCount
			&& Row >= 0 && Row < FStashGridConfig::Rows
			&& Col >= 0 && Col < FStashGridConfig::Cols;
	}
};

USTRUCT(BlueprintType)
struct SURVIVAL_API FStashItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stash")
	FGuid ItemId = FGuid::NewGuid();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stash")
	uint8 MainCategory = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stash")
	uint8 SubCategory = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stash")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stash")
	int32 Quantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stash")
	int32 MaxStack = 1;
};

USTRUCT(BlueprintType)
struct SURVIVAL_API FStashSlotData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stash")
	bool bOccupied = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stash")
	FStashItemData Item;
};

USTRUCT(BlueprintType)
struct SURVIVAL_API FStashPageData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stash")
	TArray<FStashSlotData> Slots;
};
