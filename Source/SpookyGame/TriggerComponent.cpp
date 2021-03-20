// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"
#include "Interaction.h"

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
	FTimerDelegate TimerDel;
	TimerDel.BindUObject(this, &UTriggerComponent::ExecuteInteraction, Instigator);

	bool bCanTrigger = TriggerCount < TriggerAmount || TriggerAmount == 0;
	if (bCanTrigger)
	{
		FTimerManager TimeManager = GetOwner()->GetWorldTimerManager();
		if (TriggerDelay > 0.f)
		{
			/** Only apply timer if its not already set */
			if (!TimeManager.IsTimerActive(DelayHandle))
				TimeManager.SetTimer(DelayHandle, TimerDel, TriggerDelay, false);
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
	for (AActor* Actor : ActorsToTrigger)
	{
		/** Call trigger function for all actors in array if they implement the trigger interface */
		if (IsValid(Actor) && Actor->Implements<UInteraction>())
		{
			if (IInteraction::Execute_CanInteract(Actor))
			{
				IInteraction::Execute_OnInteract(Actor, Instigator);
			}
		}
	}
}

