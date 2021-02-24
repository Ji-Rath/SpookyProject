// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "PlayerEquipComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Pickupable.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UInventoryComponent::DropItemFromSlot(int Slot, int Count)
{
	APawn* Player = GetOwner<APawn>();
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	APickupable* DroppedItem = GetWorld()->SpawnActor<APickupable>(Inventory[Slot].ItemData->ActorClass, GetOwner()->GetActorLocation(), FRotator::ZeroRotator);

	DroppedItem->Amount = Count;

	RemoveFromInventory(Slot, Count);
}

void UInventoryComponent::RemoveFromInventory(int ItemSlot, const int Count)
{
	/** If there is an item at the slot, remove specified amount */
	if (Inventory.IsValidIndex(ItemSlot))
	{
		Inventory[ItemSlot].Count -= Count;
		if (Inventory[ItemSlot].Count <= 0)
			Inventory.RemoveAt(ItemSlot);
		OnInventoryChange.Broadcast(false, ItemSlot);
	}
}

int UInventoryComponent::FindItemSlot(UItemData* Item) const
{
	/** Find slot with item in it */
	for (int i = 0; i < Inventory.Num(); i++)
	{
		if (Item->Name.ToString() == Inventory[i].ItemData->Name.ToString())
		{
			return i;
		}
	}
	/** Search failed */
	return -1;
}

bool UInventoryComponent::AddToInventory(UItemData* Item, const int Count)
{
	for (int i=0;i<Inventory.Num();i++)
	{
		FInventoryContents& InventoryContent = Inventory[i];
		/** Compare names to see if they are the same item */
		if (InventoryContent.ItemData->Name.ToString() == Item->Name.ToString())
		{
			/** Ensure that adding the item will not exceed the max stack */
			if (InventoryContent.Count + Count <= InventoryContent.ItemData->MaxStack)
			{
				InventoryContent.Count += Count;
				OnInventoryChange.Broadcast(true, i);
				return true;
			}
		}
	}

	if (Inventory.Num() < InventorySize)
	{
		/** Just add the item to a new slot */
		FInventoryContents InventoryItem;
		InventoryItem.ItemData = Item;
		InventoryItem.Count = Count;

		int Slot = Inventory.Add(InventoryItem);
		OnInventoryChange.Broadcast(true, Slot);

		return true;
	}
	
	return false;
}

void UInventoryComponent::GetInventory(TArray<FInventoryContents>& OutInventory) const
{
	OutInventory = Inventory;
}

