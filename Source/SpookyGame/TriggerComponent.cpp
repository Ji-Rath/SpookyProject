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
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTriggerComponent::TriggerActors()
{
	AInteractableBase* InteractActor;

	for (AActor* Actor : ActorsToTrigger)
	{
		//Call trigger function for all actors in array if they implement the trigger interface
		if (IsValid(Actor) && UKismetSystemLibrary::DoesImplementInterface(Actor, UTriggerInterface::StaticClass()))
		{
			InteractActor = Cast<AInteractableBase>(Actor);
			if (IsValid(InteractActor))
			{
				if (InteractActor->bCanInteract)
				{
					ITriggerInterface::Execute_OnTrigger(Actor);
					if (InteractActor->bOneTimeInteraction)
						InteractActor->bCanInteract = false;
				}
			}
			else
				ITriggerInterface::Execute_OnTrigger(Actor);
		}
	}

	//Trigger interact with owning actor if possible
	if (TriggerSelf && UKismetSystemLibrary::DoesImplementInterface(GetOwner(), UTriggerInterface::StaticClass()))
	{
		InteractActor = Cast<AInteractableBase>(GetOwner());
		if (IsValid(InteractActor))
		{
			if (InteractActor->bCanInteract)
			{
				ITriggerInterface::Execute_OnTrigger(GetOwner());
				if (InteractActor->bOneTimeInteraction)
					InteractActor->bCanInteract = false;
			}
		}
		else
			ITriggerInterface::Execute_OnTrigger(GetOwner());
		
	}
		
}

