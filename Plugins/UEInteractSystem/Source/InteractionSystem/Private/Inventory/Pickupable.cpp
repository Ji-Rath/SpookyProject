#pragma once

#include "Inventory/Pickupable.h"
#include "Inventory/InventoryComponent.h"
#include "Interaction/TriggerComponent.h"
#include "Components/StaticMeshComponent.h"



APickupable::APickupable()
{
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));

	bUseData = true;
	ItemMesh->SetGenerateOverlapEvents(false);
	ItemMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	ItemMesh->SetSimulatePhysics(true);
	ItemMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}

void APickupable::PickupItem(AActor* Interactor)
{
	UInventoryComponent* InventoryRef = Interactor->FindComponentByClass<UInventoryComponent>();

	/** Attempt to add the item to the inventory, destroy the item if successful */
	if (InventoryRef && ensureMsgf(ItemData, TEXT("Cannot add item to inventory without ItemData!")))
	{
		bool Success = InventoryRef->AddToInventory(ItemData, Amount);
		if (Success)
			Destroy();
	}
}

void APickupable::OnUseItem_Implementation(AActor* User)
{
	//Blank default implementation
}

void APickupable::OnInteract_Implementation(AActor* Interactor)
{
	PickupItem(Interactor);
}
