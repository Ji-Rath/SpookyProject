// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInteractComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"
#include "TriggerComponent.h"
#include "Interaction.h"


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
	AActor* HitActor = Hit.GetActor();

	/** Return early if there is no change */
	if (HitActor == InteractHover) { return; }

	if (bHitInteractable && HitActor)
	{
		if (HitActor->Implements<UInteraction>())
		{
			/** Set interact message when hovering over an interactable */
			if (IInteraction::Execute_CanPlayerInteract(HitActor))
			{
				InteractHover = HitActor;
				OnUpdateInteract.Broadcast(true, InteractHover);
				return;
			}
		}
	}
	
	
	/** Send interaction update when there is no longer an interactable in view */
	if (InteractHover && (!HitActor || (HitActor && !HitActor->Implements<UInteraction>())))
	{
		InteractHover = nullptr;
		OnUpdateInteract.Broadcast(false, nullptr);
	}
}

void UPlayerInteractComponent::Interact()
{
	if (InteractHover && IInteraction::Execute_CanPlayerInteract(InteractHover))
	{
		/** Trigger interacted actor */
		OnInteract.Broadcast(InteractHover);
		IInteraction::Execute_OnInteract(InteractHover, GetOwner());

		/** Call trigger actors from component */
		UTriggerComponent* TriggerComponent = InteractHover->FindComponentByClass<UTriggerComponent>();
		if (TriggerComponent)
			TriggerComponent->TriggerActors(GetOwner());
	}
}