// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"
#include "ItemData.h"
#include "TriggerComponent.h"

// Sets default values
AInteractable::AInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AInteractable::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (bUseData && IsValid(ItemData))
	{
		Name = ItemData->Name;
	}
}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();

	OnInteracted.AddDynamic(this, &AInteractable::OnInteract);
}

void AInteractable::OnInteract_Implementation(AActor* Interactor)
{
	//Blank implementation
}

bool AInteractable::Interact(AActor* Interactor)
{
	bool bReachedInteractLimit = !(InteractAmount == -1 || (InteractCount < InteractAmount));
	if (bCanInteract && CanInteract(Interactor) && !bReachedInteractLimit)
	{
		InteractCount++;
		OnInteracted.Broadcast(Interactor);

		return true;
	}
	return false;
}

bool AInteractable::CanInteract_Implementation(AActor* Interactor) const
{
	return true;
}

void AInteractable::SetInteractable(bool bInteractable)
{
	bCanInteract = bInteractable;
}

FText AInteractable::GetName() const 
{
	return Name;
}

UItemData* AInteractable::GetItemData() const
{
	return ItemData;
}

