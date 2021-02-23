// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
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


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::EquipSlot(int ItemSlot)
{
	UnequipItem();

	if (IsValid(Inventory[ItemSlot].ItemData))
	{
		TSubclassOf<APickupable> ActorClass = Inventory[ItemSlot].ItemData->ActorClass;
		APickupable* Interactable = GetWorld()->SpawnActor<APickupable>(ActorClass, GetOwner()->GetTransform());
		FAttachmentTransformRules TransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);

		Interactable->AttachToComponent(Cast<USceneComponent>(ItemAttachParent.GetComponent(GetOwner())), TransformRules);
		Interactable->SetActorEnableCollision(false);
		Interactable->GetItemMesh()->SetSimulatePhysics(false);
		EquippedSlot = ItemSlot;
	}
}

void UInventoryComponent::UnequipItem()
{
	USceneComponent* ItemAttach = Cast<USceneComponent>(ItemAttachParent.GetComponent(GetOwner()));
	if (ensureMsgf(ItemAttach, TEXT("ItemAttach component is not a scene component!")))
	{
		/** Unequip any items that were binded to the actor */
		TArray<AActor *> ItemsAttached;
		GetOwner()->GetAttachedActors(OUT ItemsAttached);
		for (AActor* Item : ItemsAttached)
		{
			Item->Destroy();
		}
		EquippedSlot = 0;
	}
}

void UInventoryComponent::DropItemFromSlot(int Slot, int Count)
{
	APickupable* DroppedItem = GetWorld()->SpawnActor<APickupable>(Inventory[Slot].ItemData->ActorClass, GetOwner()->GetTransform());
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
			if (EquippedSlot == ItemSlot)
			{
				UnequipItem();
			}
		}
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

int UInventoryComponent::GetEquippedSlot() const
{
	return EquippedSlot;
}

void UInventoryComponent::GetEquippedSlotItem(FInventoryContents& InventorySlot) const
{
	if (GetEquippedSlot() != -1)
	{
		InventorySlot = Inventory[GetEquippedSlot()];
	}
	else
	{
		InventorySlot = FInventoryContents();
	}
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

	if (Inventory.Num() < InventorySize)
	{
		/** Just add the item to a new slot */
		FInventoryContents InventoryItem;
		InventoryItem.ItemData = Item;
		InventoryItem.Count = Count;

		int Slot = Inventory.Add(InventoryItem);
		OnInventoryChange.Broadcast(true);

		EquipSlot(Slot);
		return true;
	}
	
	return false;
}

void UInventoryComponent::GetInventory(TArray<FInventoryContents>& InvContents) const
{
	InvContents = Inventory;
}

