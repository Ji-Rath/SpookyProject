// Fill out your copyright notice in the Description page of Project Settings.


#include "ToggleInteractable.h"

#include "Interaction/ToggleInteractableComponent.h"


// Sets default values
AToggleInteractable::AToggleInteractable(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UToggleInteractableComponent>(AInteractable::InteractableComponentName))
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AToggleInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AToggleInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UToggleInteractableComponent* AToggleInteractable::GetToggleInteractableComponent() const
{
	return Cast<UToggleInteractableComponent>(InteractableComponent);
}

