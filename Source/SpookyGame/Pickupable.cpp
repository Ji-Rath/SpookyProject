// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickupable.h"

#include "Inventory/PickupableComponent.h"

APickupable::APickupable(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UPickupableComponent>(AInteractable::InteractableComponentName))
{
	
}

UPickupableComponent* APickupable::GetPickupableComponent() const
{
	return Cast<UPickupableComponent>(InteractableComponent);
}
