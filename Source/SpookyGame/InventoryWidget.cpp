// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"

#include "InteractionSystem_Settings.h"
#include "Components/ScrollBox.h"
#include "Inventory/InventoryComponent.h"
#include "InventoryItemWidget.h"
#include "Inventory/PlayerEquipComponent.h"

bool UInventoryWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }

	if (!InventoryRef)
	{
		if (APawn* Player = GetOwningPlayerPawn())
		{
			InventoryRef = Player->FindComponentByClass<UInventoryComponent>();
		}	
	}

	// Bind UpdateInventory to OnInventoryChange so it is called everytime there is an inventory update
	if (InventoryRef)
	{
		InventoryRef->OnInventoryChange.AddDynamic(this, &UInventoryWidget::UpdateInventory);
		UpdateInventory(false);
	}
	
	return true;
}

void UInventoryWidget::UpdateInventory(bool bItemAdded)
{
	if (ensure(InventoryRef && ItemWidget))
	{
		// Clear all items from display
		InventoryDisplay->ClearChildren();

		// Get player inventory contents
		int CurrentSlot = 0;
		TArray<FInventoryContents> Inventory;
		InventoryRef->GetInventory(Inventory);

		// Loop through all current items in player inventory and display them to UI
		for (const FInventoryContents& InventoryItem : Inventory)
		{
			const FItemInfo* ItemInfo = InventoryItem.GetRow<FItemInfo>("");
			if (ensure(ItemInfo && InventoryDisplay))
			{
				UInventoryItemWidget* ItemReference = CreateWidget<UInventoryItemWidget>(InventoryDisplay, ItemWidget);
				ItemReference->UpdateDisplay(ItemInfo->DisplayName, InventoryItem.Count);
				InventoryDisplay->AddChild(ItemReference);
				ItemReference->ItemSlot = CurrentSlot;
				ItemReference->OnClick.AddDynamic(this, &UInventoryWidget::ClickedOnItem);
				CurrentSlot++;
			}
		}
	}
}

void UInventoryWidget::ClickedOnItem(int ItemIndex)
{
	OnItemClick.Broadcast(ItemIndex, InventoryRef);
}

void UInventoryWidget::ToggleItem(int ItemSlot)
{
	UPlayerEquipComponent* EquipCompRef = nullptr;
	
	if (GetOwningPlayerPawn())
	{
		EquipCompRef = GetOwningPlayerPawn()->FindComponentByClass<UPlayerEquipComponent>();
	}
	
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

