// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInteractComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"
#include "TriggerComponent.h"
#include "InteractableBase.h"
#include "MainUIBase.h"


// Sets default values for this component's properties
UPlayerInteractComponent::UPlayerInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.25;

	// ...
}


// Called when the game starts
void UPlayerInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HoverInteraction(DeltaTime);
}

void UPlayerInteractComponent::HoverInteraction(float DeltaTime)
{
	/** Ensure that the owner is a pawn */
	APawn* Player = GetOwner<APawn>();
	if (!Player) return;
	APlayerController* PlayerController = Player->GetController<APlayerController>();

	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(Player->GetControlRotation());
	FVector Distance = ForwardVector * InteractDistance;
	FCollisionQueryParams QueryParams = FCollisionQueryParams(FName(TEXT("Interaction Actor")), false, Player);
	FHitResult Hit;
	FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
	

	//Line trace for interactable objects
	if (GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, CameraLocation + Distance, ECC_Visibility, QueryParams))
	{
		/** Set interact message when hovering over an interactable */
		InteractHover = Cast<AInteractableBase>(Hit.GetActor());
		if (IsValid(InteractHover) && InteractHover->CanInteract())
		{
			FString Message = InteractMessage + InteractHover->Name.ToString();
			OnUpdateInteract.Broadcast(Message);
			return;
		}
	}

	//If there is no interactable traced, send a blank message
	InteractHover = nullptr;
	OnUpdateInteract.Broadcast(TEXT(""));
}

void UPlayerInteractComponent::Interact()
{
	if (IsValid(InteractHover) && InteractHover->bPlayerInteract)
	{
		//Call TriggerActors from TriggerComponent 
		UTriggerComponent* TriggerComponent = InteractHover->FindComponentByClass<UTriggerComponent>();
		if (TriggerComponent != nullptr)
		{
			TriggerComponent->TriggerActors();
		}
	}
}