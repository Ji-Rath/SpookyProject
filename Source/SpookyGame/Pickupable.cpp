#pragma once

#include "Pickupable.h"
#include "InventoryComponent.h"
#include "TriggerComponent.h"
#include "Components/StaticMeshComponent.h"



APickupable::APickupable()
{
	TriggerComp = CreateDefaultSubobject<UTriggerComponent>(TEXT("Trigger Comp"));
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));

	bUseData = true;
	ItemMesh->SetGenerateOverlapEvents(false);
	ItemMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	ItemMesh->SetSimulatePhysics(true);
	ItemMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}

void APickupable::OnInteract_Implementation(AActor* Interactor)
{
	UInventoryComponent* InventoryRef = Interactor->FindComponentByClass<UInventoryComponent>();

	/** Attempt to add the item to the inventory, destroy the item if successful */
	if (InventoryRef)
	{
		bool Success = InventoryRef->AddToInventory(ItemData, Amount);
		if (Success)
			Destroy();
	}
	
}
