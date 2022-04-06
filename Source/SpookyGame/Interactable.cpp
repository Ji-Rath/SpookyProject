// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"

FName AInteractable::InteractableComponentName(TEXT("InteractableComponent"));

// Sets default values
AInteractable::AInteractable(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(AInteractable::InteractableComponentName);
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

	// Fix TMaps not being compatible with the save system by turning them into TArrays right before saving
	if (InteractableComponent)
	{
		ActorFilters.Empty();
		ActorBoolFilters.Empty();
		ActorFilters.Reserve(InteractableComponent->InteractFilters.Num());
		ActorBoolFilters.Reserve(InteractableComponent->InteractFilters.Num());
		
		for (const TPair<TSubclassOf<AActor>, bool>& pair : InteractableComponent->InteractFilters)
		{
			ActorFilters.Emplace(pair.Key);
			ActorBoolFilters.Emplace(pair.Value);
		}
	}
}

void AInteractable::SpudPostRestore_Implementation(const USpudState* State)
{
	ISpudObjectCallback::SpudPostRestore_Implementation(State);

	// Restore TMap data by retreiving data stored in the TArrays
	if (InteractableComponent)
	{
		InteractableComponent->InteractFilters.Empty();
		InteractableComponent->InteractFilters.Reserve(ActorFilters.Num());
		
		for (int i=0;i<ActorFilters.Num();i++)
		{
			InteractableComponent->InteractFilters.Emplace(ActorFilters[i], ActorBoolFilters[i]);
		}
	}
}

UInteractableComponent* AInteractable::GetInteractableComponent() const
{
	return InteractableComponent;
}
