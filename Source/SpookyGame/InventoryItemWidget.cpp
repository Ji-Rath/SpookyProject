
#include "InventoryItemWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Inventory/PlayerEquipComponent.h"
#include "Inventory/InventoryComponent.h"
#include "Interaction/ItemData.h"

bool UInventoryItemWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }

	if (GetOwningPlayerPawn())
	{
		EquipCompRef = GetOwningPlayerPawn()->FindComponentByClass<UPlayerEquipComponent>();
		InventoryRef = GetOwningPlayerPawn()->FindComponentByClass<UInventoryComponent>();
	}

	if (ItemButton)
		ItemButton->OnClicked.AddDynamic(this, &UInventoryItemWidget::ToggleItem);
	return true;
}

void UInventoryItemWidget::UpdateDisplay(const FText& Name, const int& Amount)
{
	if (ensure(ItemName && ItemAmount))
	{
		ItemName->SetText(Name);
		ItemAmount->SetText(FText::AsNumber(Amount));
	}
}

void UInventoryItemWidget::ToggleItem()
{
	if (EquipCompRef)
	{
		/** Equip current item in slot if not already done */
		const FInventoryContents Item = InventoryRef->FindItem(ItemSlot);
		if (EquipCompRef->GetEquippedItemData().IsNull())
		{
			EquipCompRef->EquipItem(Item);
		}
		else 
		{
			if (Item == EquipCompRef->GetEquippedItemData())
			{
				EquipCompRef->UnequipItem();
			}
			else
			{
				EquipCompRef->EquipItem(Item);
			}
		}
	}
}
