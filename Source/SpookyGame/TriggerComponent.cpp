// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"
#include "TriggerInterface.h"

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
}

void UTriggerComponent::TriggerActors(AActor* Instigator)
{
	for (AActor* Actor : ActorsToTrigger)
	{
		/** Call trigger function for all actors in array if they implement the trigger interface */
		if (IsValid(Actor) && Actor->Implements<UTriggerInterface>())
		{
			if (ITriggerInterface::Execute_CanTrigger(Actor))
			{
				ITriggerInterface::Execute_OnTrigger(Actor, Instigator);
				// InteractActor->ToggleOnStatus();
			}
			else
			{
				/** Handle cases where the object is not a child of AInteractableBase */
				ITriggerInterface::Execute_OnTrigger(Actor, Instigator);
			}
		}
	}
}

