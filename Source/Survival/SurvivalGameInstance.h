#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Survival/Inventory/StashTypes.h"
#include "SurvivalGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStashChangedSignature, int32, PageIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStashUiRequestedSignature);

UCLASS()
class SURVIVAL_API USurvivalGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	UPROPERTY(BlueprintAssignable, Category = "Stash")
	FOnStashChangedSignature OnStashChanged;

	UPROPERTY(BlueprintAssignable, Category = "Stash")
	FOnStashUiRequestedSignature OnStashUiRequested;

	UFUNCTION(BlueprintCallable, Category = "Stash")
	void InitializeStashIfNeeded();

	UFUNCTION(BlueprintCallable, Category = "Stash")
	bool LoadStash();

	UFUNCTION(BlueprintCallable, Category = "Stash")
	bool SaveStash();

	UFUNCTION(BlueprintCallable, Category = "Stash")
	const TArray<FStashSlotData>& BP_GetStashPage(int32 PageIndex) const;

	UFUNCTION(BlueprintCallable, Category = "Stash")
	bool BP_CanMoveItem(const FStashCellRef& FromCell, const FStashCellRef& ToCell) const;

	UFUNCTION(BlueprintCallable, Category = "Stash")
	bool BP_MoveItem(const FStashCellRef& FromCell, const FStashCellRef& ToCell);

	UFUNCTION(BlueprintCallable, Category = "Stash")
	bool BP_CanPlaceItem(const FStashCellRef& TargetCell, const FStashItemData& Item) const;

	UFUNCTION(BlueprintCallable, Category = "Stash")
	bool BP_AddItemAt(const FStashCellRef& TargetCell, const FStashItemData& Item);

	UFUNCTION(BlueprintCallable, Category = "Stash")
	bool BP_RemoveItemAt(const FStashCellRef& TargetCell);

	UFUNCTION(BlueprintCallable, Category = "Stash")
	void BP_RequestOpenStashUi();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stash")
	FString StashSlotName = TEXT("StashSlot");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stash")
	int32 StashUserIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stash")
	bool bAutoSaveOnMutation = true;

private:
	UPROPERTY()
	TArray<FStashPageData> StashPages;

	UPROPERTY()
	FStashPageData EmptyPageFallback;

	bool IsValidCell(const FStashCellRef& Cell) const;
	int32 ToLinearIndex(const FStashCellRef& Cell) const;

	FStashSlotData* FindSlotMutable(const FStashCellRef& Cell);
	const FStashSlotData* FindSlot(const FStashCellRef& Cell) const;

	bool CanPlaceAt(const FStashCellRef& Cell) const;
	void BroadcastPageChangedIfValid(int32 PageIndex);
	void CreateEmptyPages();
};
