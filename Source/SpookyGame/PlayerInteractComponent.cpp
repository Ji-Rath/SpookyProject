// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInteractComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Interactable.h"


// Sets default values for this component's properties
UPlayerInteractComponent::UPlayerInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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
	if (!PlayerController) return;

	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(Player->GetControlRotation());
	FVector Distance = ForwardVector * InteractDistance;
	FCollisionQueryParams QueryParams = FCollisionQueryParams(FName(TEXT("Interaction Actor")), false, Player);
	FHitResult Hit;
	FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();

	bool bHitInteractable = GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, CameraLocation + Distance, ECC_Visibility, QueryParams);
	AInteractable* HitActor = Cast<AInteractable>(Hit.GetActor());

	/** Return early if there is no change */
	if (HitActor == InteractHover) { return; }

	if (bHitInteractable && HitActor)
	{
		/** Set interact message when hovering over an interactable */
		if (HitActor->CanInteract(GetOwner()))
		{
			InteractHover = HitActor;
			OnUpdateInteract.Broadcast(true, InteractHover);
			return;
		}
	}
	
	
	/** Send interaction update when there is no longer an interactable in view */
	if (InteractHover && !HitActor)
	{
		InteractHover = nullptr;
		OnUpdateInteract.Broadcast(false, nullptr);
	}
}

void UPlayerInteractComponent::Interact()
{
	// Allows functions to use OnInteract without player looking at interactable (ex. using equipped item)
	OnInteract.Broadcast(InteractHover);

	if (InteractHover)
	{
		/** Trigger interacted actor */
		InteractHover->Interact(GetOwner());
	}
}