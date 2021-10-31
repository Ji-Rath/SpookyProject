// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/TriggerComponent.h"
#include "Interaction/Interactable.h"

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

	if (AInteractable* Owner = GetOwner<AInteractable>())
	{
		Owner->OnInteracted.AddDynamic(this, &UTriggerComponent::TriggerActors);
	}
}

void UTriggerComponent::TriggerActors(AActor* Instigator)
{
	FTimerDelegate TimerDel;
	TimerDel.BindUObject(this, &UTriggerComponent::ExecuteInteraction, Instigator);

	bool bCanTrigger = TriggerCount < TriggerAmount || TriggerAmount == 0;
	if (bCanTrigger)
	{
		if (TriggerDelay > 0.f)
		{
			/** Only apply timer if its not already set */
			if (!GetOwner()->GetWorldTimerManager().IsTimerActive(DelayHandle))
				GetOwner()->GetWorldTimerManager().SetTimer(DelayHandle, TimerDel, TriggerDelay, false);
		}
		else
		{
			/** Trigger actors instantly if there is no delay set */
			TimerDel.ExecuteIfBound();
		}

		TriggerCount++;
	}
}

void UTriggerComponent::ExecuteInteraction(AActor* Instigator)
{
	for (AInteractable* Interactable : ActorsToTrigger)
	{
		/** Call trigger function for all actors in array if they implement the trigger interface */
		Interactable->Interact(Instigator);
	}
}

