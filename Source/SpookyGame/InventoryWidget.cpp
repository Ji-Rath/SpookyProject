// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "Components/ScrollBox.h"
#include "InventoryComponent.h"
#include "InventoryItemWidget.h"

bool UInventoryWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }

	if (GetOwningPlayerPawn())
		InventoryRef = GetOwningPlayerPawn()->FindComponentByClass<UInventoryComponent>();
	return true;
}

void UInventoryWidget::UpdateInventory()
{
	if (InventoryRef && ItemWidget)
	{
		InventoryDisplay->ClearChildren();
		for (const FInventoryContents& InventoryItem : InventoryRef->Inventory)
		{
			UInventoryItemWidget* ItemReference = CreateWidget<UInventoryItemWidget>(InventoryDisplay, ItemWidget);
			ItemReference->UpdateDisplay(InventoryItem.ItemData->Name, InventoryItem.Count);
			InventoryDisplay->AddChild(ItemReference);
		}
	}
}
