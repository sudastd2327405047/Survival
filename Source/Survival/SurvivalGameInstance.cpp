#include "SurvivalGameInstance.h"

#include "Survival/Inventory/SurvivalStashSaveGame.h"
#include "Kismet/GameplayStatics.h"

void USurvivalGameInstance::Init()
{
	Super::Init();
	InitializeStashIfNeeded();
	LoadStash();
}

void USurvivalGameInstance::InitializeStashIfNeeded()
{
	if (StashPages.Num() == FStashGridConfig::PageCount)
	{
		return;
	}

	CreateEmptyPages();
}

bool USurvivalGameInstance::LoadStash()
{
	InitializeStashIfNeeded();

	if (!UGameplayStatics::DoesSaveGameExist(StashSlotName, StashUserIndex))
	{
		// 中文注释：首次进入没有存档时，直接用空仓库作为回退数据。
		CreateEmptyPages();
		return SaveStash();
	}

	USaveGame* Loaded = UGameplayStatics::LoadGameFromSlot(StashSlotName, StashUserIndex);
	USurvivalStashSaveGame* LoadedStash = Cast<USurvivalStashSaveGame>(Loaded);
	if (!LoadedStash || LoadedStash->SavedPages.Num() != FStashGridConfig::PageCount)
	{
		// 中文注释：存档损坏或版本不匹配时，重建默认仓库避免UI崩溃。
		CreateEmptyPages();
		return SaveStash();
	}

	StashPages = LoadedStash->SavedPages;
	for (int32 PageIndex = 0; PageIndex < StashPages.Num(); ++PageIndex)
	{
		if (StashPages[PageIndex].Slots.Num() != FStashGridConfig::SlotsPerPage)
		{
			CreateEmptyPages();
			return SaveStash();
		}
		BroadcastPageChangedIfValid(PageIndex);
	}

	return true;
}

bool USurvivalGameInstance::SaveStash()
{
	InitializeStashIfNeeded();

	USurvivalStashSaveGame* SaveGameObject = Cast<USurvivalStashSaveGame>(
		UGameplayStatics::CreateSaveGameObject(USurvivalStashSaveGame::StaticClass()));
	if (!SaveGameObject)
	{
		return false;
	}

	SaveGameObject->SavedPages = StashPages;
	return UGameplayStatics::SaveGameToSlot(SaveGameObject, StashSlotName, StashUserIndex);
}

const TArray<FStashSlotData>& USurvivalGameInstance::BP_GetStashPage(int32 PageIndex) const
{
	if (!StashPages.IsValidIndex(PageIndex))
	{
		return EmptyPageFallback.Slots;
	}

	return StashPages[PageIndex].Slots;
}

bool USurvivalGameInstance::BP_CanMoveItem(const FStashCellRef& FromCell, const FStashCellRef& ToCell) const
{
	if (!IsValidCell(FromCell) || !IsValidCell(ToCell))
	{
		return false;
	}

	if (FromCell.Page == ToCell.Page && FromCell.Row == ToCell.Row && FromCell.Col == ToCell.Col)
	{
		return false;
	}

	const FStashSlotData* FromSlot = FindSlot(FromCell);
	const FStashSlotData* ToSlot = FindSlot(ToCell);
	if (!FromSlot || !ToSlot)
	{
		return false;
	}

	// 中文注释：拖拽目标格必须为空，直接禁止重叠放置。
	return FromSlot->bOccupied && !ToSlot->bOccupied;
}

bool USurvivalGameInstance::BP_MoveItem(const FStashCellRef& FromCell, const FStashCellRef& ToCell)
{
	InitializeStashIfNeeded();

	if (!BP_CanMoveItem(FromCell, ToCell))
	{
		return false;
	}

	FStashSlotData* FromSlot = FindSlotMutable(FromCell);
	FStashSlotData* ToSlot = FindSlotMutable(ToCell);
	if (!FromSlot || !ToSlot)
	{
		return false;
	}

	ToSlot->Item = FromSlot->Item;
	ToSlot->bOccupied = true;
	FromSlot->Item = FStashItemData();
	FromSlot->bOccupied = false;

	BroadcastPageChangedIfValid(FromCell.Page);
	if (FromCell.Page != ToCell.Page)
	{
		BroadcastPageChangedIfValid(ToCell.Page);
	}

	if (bAutoSaveOnMutation)
	{
		SaveStash();
	}

	return true;
}

bool USurvivalGameInstance::BP_CanPlaceItem(const FStashCellRef& TargetCell, const FStashItemData& Item) const
{
	if (!IsValidCell(TargetCell))
	{
		return false;
	}

	if (Item.Quantity <= 0 || Item.MaxStack <= 0)
	{
		return false;
	}

	return CanPlaceAt(TargetCell);
}

bool USurvivalGameInstance::BP_AddItemAt(const FStashCellRef& TargetCell, const FStashItemData& Item)
{
	InitializeStashIfNeeded();

	if (!BP_CanPlaceItem(TargetCell, Item))
	{
		return false;
	}

	FStashSlotData* TargetSlot = FindSlotMutable(TargetCell);
	if (!TargetSlot)
	{
		return false;
	}

	TargetSlot->Item = Item;
	TargetSlot->bOccupied = true;
	BroadcastPageChangedIfValid(TargetCell.Page);

	if (bAutoSaveOnMutation)
	{
		SaveStash();
	}

	return true;
}

bool USurvivalGameInstance::BP_RemoveItemAt(const FStashCellRef& TargetCell)
{
	InitializeStashIfNeeded();

	FStashSlotData* TargetSlot = FindSlotMutable(TargetCell);
	if (!TargetSlot || !TargetSlot->bOccupied)
	{
		return false;
	}

	TargetSlot->Item = FStashItemData();
	TargetSlot->bOccupied = false;
	BroadcastPageChangedIfValid(TargetCell.Page);

	if (bAutoSaveOnMutation)
	{
		SaveStash();
	}

	return true;
}

void USurvivalGameInstance::BP_RequestOpenStashUi()
{
	// 中文注释：打开仓库前确保运行态数据可用，避免UI读到空引用。
	InitializeStashIfNeeded();
	OnStashUiRequested.Broadcast();
}

bool USurvivalGameInstance::IsValidCell(const FStashCellRef& Cell) const
{
	return Cell.IsValid();
}

int32 USurvivalGameInstance::ToLinearIndex(const FStashCellRef& Cell) const
{
	// 中文注释：二维网格映射到线性数组，便于序列化和高效访问。
	return Cell.Row * FStashGridConfig::Cols + Cell.Col;
}

FStashSlotData* USurvivalGameInstance::FindSlotMutable(const FStashCellRef& Cell)
{
	if (!IsValidCell(Cell) || !StashPages.IsValidIndex(Cell.Page))
	{
		return nullptr;
	}

	FStashPageData& Page = StashPages[Cell.Page];
	const int32 SlotIndex = ToLinearIndex(Cell);
	if (!Page.Slots.IsValidIndex(SlotIndex))
	{
		return nullptr;
	}

	return &Page.Slots[SlotIndex];
}

const FStashSlotData* USurvivalGameInstance::FindSlot(const FStashCellRef& Cell) const
{
	if (!IsValidCell(Cell) || !StashPages.IsValidIndex(Cell.Page))
	{
		return nullptr;
	}

	const FStashPageData& Page = StashPages[Cell.Page];
	const int32 SlotIndex = ToLinearIndex(Cell);
	if (!Page.Slots.IsValidIndex(SlotIndex))
	{
		return nullptr;
	}

	return &Page.Slots[SlotIndex];
}

bool USurvivalGameInstance::CanPlaceAt(const FStashCellRef& Cell) const
{
	const FStashSlotData* TargetSlot = FindSlot(Cell);
	return TargetSlot && !TargetSlot->bOccupied;
}

void USurvivalGameInstance::BroadcastPageChangedIfValid(int32 PageIndex)
{
	if (PageIndex >= 0 && PageIndex < FStashGridConfig::PageCount)
	{
		OnStashChanged.Broadcast(PageIndex);
	}
}

void USurvivalGameInstance::CreateEmptyPages()
{
	StashPages.Empty();
	StashPages.SetNum(FStashGridConfig::PageCount);
	for (FStashPageData& Page : StashPages)
	{
		Page.Slots.Empty();
		Page.Slots.SetNum(FStashGridConfig::SlotsPerPage);
	}

	EmptyPageFallback.Slots.Empty();
	EmptyPageFallback.Slots.SetNum(FStashGridConfig::SlotsPerPage);
}
