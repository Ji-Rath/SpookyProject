
#include "InventoryItemWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "PlayerEquipComponent.h"
#include "InventoryComponent.h"
#include "ItemData.h"

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
	ItemName->SetText(Name);
	ItemAmount->SetText(FText::AsNumber(Amount));
}

void UInventoryItemWidget::ToggleItem()
{
	if (EquipCompRef)
	{
		/** Equip current item in slot if not already done */
		UItemData* Item = InventoryRef->FindItem(ItemSlot);
		if (!EquipCompRef->GetEquippedItem())
		{
			EquipCompRef->EquipItem(Item);
		}
		else 
		{
			if (InventoryRef->FindItem(ItemSlot) == EquipCompRef->GetEquippedItem())
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
