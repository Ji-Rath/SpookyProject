// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

#include "TriggerInterface.h"
#include "InteractableBase.h"

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
	for (AInteractableBase* Actor : ActorsToTrigger)
	{
		//Call trigger function for all actors in array if they implement the trigger interface
		if (IsValid(Actor))
		{
			ITriggerInterface* TriggerActor = Cast<ITriggerInterface>(Actor);
			TriggerActor->Execute_OnTrigger(Actor);
		}
	}
}

