// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableBase.h"
#include "TriggerInterface.h"
#include "ItemData.h"

// Sets default values
AInteractableBase::AInteractableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AInteractableBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (bUseData && ItemData)
		Name = ItemData->Name;
}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();

}

void AInteractableBase::ToggleOnStatus()
{
	bIsOn = !bIsOn;

	/** Prevent more interactions if it should only be triggered once */
	if (bOneTimeInteraction)
		SetInteractable(false);
}

bool AInteractableBase::CanTrigger_Implementation() const
{
	return bCanInteract;
}

bool AInteractableBase::CanPlayerTrigger_Implementation() const
{
	return bCanInteract && bPlayerInteract;
}

void AInteractableBase::SetInteractable(bool bIsInteractable)
{
	bCanInteract = bIsInteractable;
}

FText AInteractableBase::GetName_Implementation() const 
{
	return Name;
}

UItemData* AInteractableBase::GetItemData() const
{
	return ItemData;
}

bool AInteractableBase::GetCurrentState() const
{
	return bIsOn;
}

