// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"
#include "ItemData.h"

// Sets default values
AInteractable::AInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AInteractable::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (bUseData && ItemData)
		Name = ItemData->Name;
}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();

}

void AInteractable::OnInteract_Implementation(AActor* Interactor)
{

}

bool AInteractable::Interact(AActor* Interactor)
{
	bool bReachedInteractLimit = InteractAmount <= 0 || (InteractCount < InteractAmount);
	if (bCanInteract && CanInteract(Interactor) && bReachedInteractLimit)
	{
		OnInteract(Interactor);
		InteractCount++;
		return true;
	}
	return false;
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

