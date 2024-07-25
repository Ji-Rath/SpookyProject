// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"

#include "Components/StaticMeshComponent.h"

FName AInteractable::InteractableComponentName(TEXT("InteractableComponent"));

// Sets default values
AInteractable::AInteractable(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(AInteractable::InteractableComponentName);
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractable::SpudPreStore_Implementation(const USpudState* State)
{
	ISpudObjectCallback::SpudPreStore_Implementation(State);
}

void AInteractable::SpudPostRestore_Implementation(const USpudState* State)
{
	ISpudObjectCallback::SpudPostRestore_Implementation(State);
}

UInteractableComponent* AInteractable::GetInteractableComponent() const
{
	return InteractableComponent;
}
