// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "InteractableBase.h"

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


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::EquipSlot(int ItemSlot)
{
	if (IsValid(Inventory[ItemSlot].ItemData))
	{
		TSubclassOf<AInteractableBase> ActorClass = Inventory[ItemSlot].ItemData->ActorClass;
		AInteractableBase* Interactable = GetWorld()->SpawnActor<AInteractableBase>(ActorClass, GetOwner()->GetTransform());
		Interactable->AttachToComponent(Cast<USceneComponent>(ItemAttachParent.GetComponent(GetOwner())), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));
	}
}

void UInventoryComponent::UnequipItem()
{
	USceneComponent* ItemAttach = Cast<USceneComponent>(ItemAttachParent.GetComponent(GetOwner()));
	TArray<USceneComponent *> ItemsAttached = ItemAttach->GetAttachChildren();
	for (USceneComponent* Item : ItemsAttached)
	{
		Item->DestroyComponent();
	}
}

void UInventoryComponent::DropItemFromSlot(int Slot, int Count)
{
	AInteractableBase* DroppedItem = GetWorld()->SpawnActor<AInteractableBase>(Inventory[Slot].ItemData->ActorClass, GetOwner()->GetTransform());
	RemoveFromInventory(Slot, Count);
}

void UInventoryComponent::RemoveFromInventory(int ItemSlot, const int Count)
{
	/** If there is an item at the slot, remove specified amount */
	if (IsValid(Inventory[ItemSlot].ItemData))
	{
		Inventory[ItemSlot].Count -= Count;
		if (Inventory[ItemSlot].Count <= 0)
		{
			Inventory.RemoveAt(ItemSlot);
		}
	}
}

int UInventoryComponent::FindItemSlot(UItemData* Item)
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
	for (FInventoryContents& InventoryContent : Inventory)
	{
		/** Compare names to see if they are the same item */
		if (InventoryContent.ItemData->Name.ToString() == Item->Name.ToString())
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

	/** Just add the item to a new slot */
	FInventoryContents InventoryItem;
	InventoryItem.ItemData = Item;
	InventoryItem.Count = Count;

	Inventory.Add(InventoryItem);
	OnInventoryChange.Broadcast(true);
	return true;
}

