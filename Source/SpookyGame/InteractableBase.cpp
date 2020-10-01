// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableBase.h"
#include "TriggerInterface.h"

// Sets default values
AInteractableBase::AInteractableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();

	if (this->Implements<UTriggerInterface>())
	{
		if (bIsOn)
			ITriggerInterface::Execute_OnTrigger(this);
	}
	
}

// Called every frame
void AInteractableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

