// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

#include "TriggerInterface.h"
#include "InteractableBase.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (TriggerSelf)
		ActorsToTrigger.Add(GetOwner());
}

void UTriggerComponent::TriggerActors()
{
	for (AActor* Actor : ActorsToTrigger)
	{
		/** Call trigger function for all actors in array if they implement the trigger interface */
		if (IsValid(Actor) && UKismetSystemLibrary::DoesImplementInterface(Actor, UTriggerInterface::StaticClass()))
		{
			if (AInteractableBase* InteractActor = Cast<AInteractableBase>(Actor))
			{
				if (InteractActor->CanInteract())
				{
					ITriggerInterface::Execute_OnTrigger(Actor);
					InteractActor->ToggleOnStatus();
				}
			}
			else
			{
				/** Handle cases where the object is not a child of AInteractableBase */
				ITriggerInterface::Execute_OnTrigger(Actor); 
			}
		}
	}
}

