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
		int CurrentSlot = 0;
		for (const FInventoryContents& InventoryItem : InventoryRef->GetInventory())
		{
			UInventoryItemWidget* ItemReference = CreateWidget<UInventoryItemWidget>(InventoryDisplay, ItemWidget);
			ItemReference->UpdateDisplay(InventoryItem.ItemData->Name, InventoryItem.Count);
			InventoryDisplay->AddChild(ItemReference);
			ItemReference->ItemSlot = CurrentSlot;
			CurrentSlot++;
		}
	}
}
