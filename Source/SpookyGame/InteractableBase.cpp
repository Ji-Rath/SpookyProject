// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableBase.h"
#include "TriggerInterface.h"
#include "ItemData.h"

// Sets default values
AInteractableBase::AInteractableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

void AInteractableBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	#if IS_EDITOR
	if (bUseData && ItemData)
	{
		Name = ItemData->Name;
	}
	#endif
}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();

	if (this->Implements<UTriggerInterface>())
	{
		if (bIsOn)
			ITriggerInterface::Execute_OnTrigger(this, this);
	}
	
}

// Called every frame
void AInteractableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableBase::ToggleOnStatus()
{
	bIsOn = !bIsOn;

	/** Prevent more interactions if it should only be triggered once */
	if (bOneTimeInteraction)
		SetInteractable(false);
}

bool AInteractableBase::CanInteract() const
{
	return bCanInteract;
}

void AInteractableBase::SetInteractable(bool Interactable)
{
	bCanInteract = Interactable;
}

FText AInteractableBase::GetName_Implementation() const
{
	return Name;
}

