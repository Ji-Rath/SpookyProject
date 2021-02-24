
#include "PlayerEquipComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "InventoryComponent.h"

void UPlayerEquipComponent::BeginPlay()
{
	Super::BeginPlay();

	ItemAttachSpring = Cast<USpringArmComponent>(ItemAttachParent.GetComponent(GetOwner()));
	if (ItemAttachSpring)
		InitialSpringArmOffset = ItemAttachSpring->TargetOffset.Z;

	InventoryCompRef = GetOwner()->FindComponentByClass<UInventoryComponent>();
	if (InventoryCompRef)
	{
		InventoryCompRef->OnInventoryChange.AddDynamic(this, &UPlayerEquipComponent::UpdateEquip);
		InventoryCompRef->GetInventory(OUT Inventory);
	}
}

UPlayerEquipComponent::UPlayerEquipComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

// Called every frame
void UPlayerEquipComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bEquippedItem && ItemAttachSpring)
	{
		float CurrentOffset = ItemAttachSpring->TargetOffset.Z;
		if (!FMath::IsNearlyEqual(CurrentOffset, InitialSpringArmOffset))
			ItemAttachSpring->TargetOffset.Z = UKismetMathLibrary::FInterpTo(CurrentOffset, InitialSpringArmOffset, DeltaTime, 5.f);
	}
}

void UPlayerEquipComponent::EquipSlot(int ItemSlot)
{
	UnequipItem();

	if (Inventory.IsValidIndex(ItemSlot))
	{
		AActor* Interactable = GetWorld()->SpawnActor<AActor>(Inventory[ItemSlot].ItemData->ActorClass, GetOwner()->GetTransform());
		FAttachmentTransformRules TransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);

		Interactable->AttachToComponent(Cast<USceneComponent>(ItemAttachParent.GetComponent(GetOwner())), TransformRules);
		Interactable->SetActorEnableCollision(false);
		if (auto* Mesh = Interactable->FindComponentByClass<UStaticMeshComponent>())
			Mesh->SetSimulatePhysics(false);
		EquippedSlot = ItemSlot;
		bEquippedItem = true;
		if (ItemAttachSpring)
			ItemAttachSpring->TargetOffset.Z = ItemUnequipOffset;
	}
}

void UPlayerEquipComponent::UnequipItem()
{
	USceneComponent* ItemAttach = Cast<USceneComponent>(ItemAttachParent.GetComponent(GetOwner()));
	if (ensureMsgf(ItemAttach, TEXT("ItemAttach component is not a scene component!")))
	{
		/** Unequip any items that were binded to the actor */
		TArray<AActor*> ItemsAttached;
		GetOwner()->GetAttachedActors(OUT ItemsAttached);
		for (AActor* Item : ItemsAttached)
		{
			Item->Destroy();
		}
		EquippedSlot = 0;
		bEquippedItem = false;
		if (ItemAttachSpring)
			ItemAttachSpring->TargetOffset.Z = InitialSpringArmOffset;
	}
}

bool UPlayerEquipComponent::HasItemEquipped()
{
	return bEquippedItem;
}

void UPlayerEquipComponent::DropEquippedItem()
{
	if (bEquippedItem)
	{
		/** Unequip any items that were binded to the actor */
		TArray<AActor*> ItemsAttached;
		GetOwner()->GetAttachedActors(OUT ItemsAttached);
		FVector ForwardVector = UKismetMathLibrary::GetForwardVector(GetOwner<APawn>()->GetControlRotation());
		for (AActor* Item : ItemsAttached)
		{
			Item->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
			Item->SetActorEnableCollision(true);
			UStaticMeshComponent* Mesh = Item->FindComponentByClass<UStaticMeshComponent>();
			if (Mesh)
			{
				Mesh->SetSimulatePhysics(true);
				Mesh->AddImpulse(ForwardVector * ThrowImpulse);
			}
		}
	}
	InventoryCompRef->RemoveFromInventory(EquippedSlot, 1);
}

void UPlayerEquipComponent::UpdateEquip(bool bAdded, int SlotChanged)
{
	InventoryCompRef->GetInventory(OUT Inventory);
	if (bAdded)
	{
		if (!bEquippedItem)
			EquipSlot(Inventory.Num()-1);
	}
	else
	{
		if (EquippedSlot == SlotChanged)
		{
			FInventoryContents InventorySlot;
			GetEquippedSlotItem(OUT InventorySlot);
			if (InventorySlot.Count == 0)
				UnequipItem();
		}
			
	}
}

void UPlayerEquipComponent::GetEquippedSlotItem(FInventoryContents& OutInventorySlot) const
{
	if (Inventory.IsValidIndex(GetEquippedSlot()))
	{
		OutInventorySlot = Inventory[GetEquippedSlot()];
	}
	else
	{
		OutInventorySlot = FInventoryContents();
	}
}

int UPlayerEquipComponent::GetEquippedSlot() const
{
	return EquippedSlot;
}
