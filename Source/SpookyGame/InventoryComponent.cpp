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

void UInventoryComponent::DropItem(const UItemData* Item, const int Count /*= 1*/)
{
	APawn* Player = GetOwner<APawn>();
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	APickupable* DroppedItem = GetWorld()->SpawnActor<APickupable>(Item->ActorClass, GetOwner()->GetActorLocation(), FRotator::ZeroRotator);

	DroppedItem->Amount = Count;

	RemoveFromInventory(Item, Count);
}

void UInventoryComponent::RemoveFromInventory(const UItemData* Item, const int Count /*= 1*/)
{
	/** If there is an item at the slot, remove specified amount */
	int Slot = FindItemSlot(Item);
	if (Inventory.IsValidIndex(Slot))
	{
		Inventory[Slot].Count -= Count;
		if (Inventory[Slot].Count <= 0)
			Inventory.RemoveAt(Slot);
		OnInventoryChange.Broadcast(false);
	}
}

int UInventoryComponent::FindItemSlot(const UItemData* Item) const
{
	/** Find slot with item in it */
	for (int i = 0; i < Inventory.Num(); i++)
	{
		if (Item == Inventory[i].ItemData)
		{
			return i;
		}
	}
	/** Search failed */
	return -1;
}

UItemData* UInventoryComponent::FindItem(const int Index) const
{
	if (Inventory.IsValidIndex(Index))
	{
		return Inventory[Index].ItemData;
	}

	return nullptr;
}

bool UInventoryComponent::AddToInventory(UItemData* Item, const int Count)
{
	for (int i=0;i<Inventory.Num();i++)
	{
		FInventoryContents& InventoryContent = Inventory[i];
		/** Compare names to see if they are the same item */
		if (InventoryContent.ItemData == Item)
		{
			/** Ensure that adding the item will not exceed the max stack */
			if (InventoryContent.Count + Count <= InventoryContent.ItemData->MaxStack)
			{
				InventoryContent.Count += Count;
				OnInventoryChange.Broadcast(true);
				return true;
			}
		}
	}

	if (Inventory.Num() < InventorySize)
	{
		/** Just add the item to a new slot */
		FInventoryContents* InventoryItem = new FInventoryContents(Item, Count);

		int Slot = Inventory.Add(*InventoryItem);
		OnInventoryChange.Broadcast(true);

		return true;
	}
	
	return false;
}

void UInventoryComponent::GetInventory(TArray<FInventoryContents>& OutInventory) const
{
	OutInventory = Inventory;
}

