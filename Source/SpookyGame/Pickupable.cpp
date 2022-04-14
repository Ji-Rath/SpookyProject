// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickupable.h"

#include "Components/StaticMeshComponent.h"
#include "Interaction/ItemData.h"
#include "Inventory/PickupableComponent.h"

APickupable::APickupable(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UPickupableComponent>(AInteractable::InteractableComponentName))
{
	
}

void APickupable::BeginPlay()
{
	Super::BeginPlay();
	
}

void APickupable::SpudStoreCustomData_Implementation(const USpudState* State, USpudStateCustomData* CustomData)
{
	Super::SpudStoreCustomData_Implementation(State, CustomData);
	
	CustomData->Write<FInventoryContents>(GetPickupableComponent()->ItemData);
}

void APickupable::SpudRestoreCustomData_Implementation(USpudState* State, USpudStateCustomData* CustomData)
{
	Super::SpudRestoreCustomData_Implementation(State, CustomData);
	CustomData->Read<FInventoryContents>(GetPickupableComponent()->ItemData);
}

void APickupable::SpudPostRestore_Implementation(const USpudState* State)
{
	Super::SpudPostRestore_Implementation(State);
	
	FItemInfo* ItemInfo = GetPickupableComponent()->ItemData.GetRow<FItemInfo>("");
	if (ItemInfo && StaticMesh)
		StaticMesh->SetStaticMesh(ItemInfo->ItemMesh.Get());
}

UPickupableComponent* APickupable::GetPickupableComponent() const
{
	return Cast<UPickupableComponent>(InteractableComponent);
}
