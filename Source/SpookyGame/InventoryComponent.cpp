// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UInventoryComponent::AddToInventory(UItemData* Item, const int Count)
{
	for (FInventoryContents& InventoryContent : Inventory)
	{
		/** Compare names to see if they are the same item */
		if (InventoryContent.ItemData->Name.ToString() == Item->Name.ToString())
		{
			/** Ensure that adding the item will not exceed the max stack */
			if (InventoryContent.Count + Count <= InventoryContent.ItemData->MaxStack)
			{
				InventoryContent.Count += Count;
				return true;
			}
		}
	}

	/** Just add the item to a new slot */
	FInventoryContents InventoryItem;
	InventoryItem.ItemData = Item;
	InventoryItem.Count = Count;

	Inventory.Add(InventoryItem);
	return true;
}

