
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

	if (EquippedItem && ItemAttachSpring)
	{
		float CurrentOffset = ItemAttachSpring->TargetOffset.Z;
		if (!FMath::IsNearlyEqual(CurrentOffset, InitialSpringArmOffset))
			ItemAttachSpring->TargetOffset.Z = UKismetMathLibrary::FInterpTo(CurrentOffset, InitialSpringArmOffset, DeltaTime, 5.f);
	}
}

void UPlayerEquipComponent::EquipItem(UItemData* Item)
{
	UnequipItem();

	if (Item)
	{
		AActor* Interactable = GetWorld()->SpawnActor<AActor>(Item->ActorClass, GetOwner()->GetTransform());
		FAttachmentTransformRules TransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);

		Interactable->AttachToComponent(Cast<USceneComponent>(ItemAttachParent.GetComponent(GetOwner())), TransformRules);
		Interactable->SetActorEnableCollision(false);
		if (auto* Mesh = Interactable->FindComponentByClass<UStaticMeshComponent>())
			Mesh->SetSimulatePhysics(false);
		EquippedItem = Item;
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
		EquippedItem = nullptr;
		if (ItemAttachSpring)
			ItemAttachSpring->TargetOffset.Z = InitialSpringArmOffset;
	}
}

UItemData* UPlayerEquipComponent::GetEquippedItem() const
{
	return EquippedItem;
}

void UPlayerEquipComponent::DropEquippedItem()
{
	if (EquippedItem)
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
	InventoryCompRef->RemoveFromInventory(EquippedItem, 1);
}

void UPlayerEquipComponent::UpdateEquip(bool bAdded)
{
	InventoryCompRef->GetInventory(OUT Inventory);
	if (bAdded)
	{
		if (!EquippedItem)
			EquipItem(Inventory[Inventory.Num()-1].ItemData);
	}
	else
	{
		if (InventoryCompRef->FindItemSlot(EquippedItem) == -1)
			UnequipItem();
	}
}
