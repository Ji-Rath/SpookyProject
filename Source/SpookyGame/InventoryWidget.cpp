// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"

#include "InteractionSystem_Settings.h"
#include "Components/ScrollBox.h"
#include "Inventory/InventoryComponent.h"
#include "InventoryItemWidget.h"

bool UInventoryWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }

	if (APawn* Player = GetOwningPlayerPawn())
	{
		InventoryRef = Player->FindComponentByClass<UInventoryComponent>();

		// Bind UpdateInventory to OnInventoryChange so it is called everytime there is an inventory update
		if (InventoryRef)
			InventoryRef->OnInventoryChange.AddDynamic(this, &UInventoryWidget::UpdateInventory);
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
				CurrentSlot++;
			}
		}
	}
}
