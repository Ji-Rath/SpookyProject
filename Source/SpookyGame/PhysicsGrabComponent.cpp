#pragma once

#include "PhysicsGrabComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values for this component's properties
UPhysicsGrabComponent::UPhysicsGrabComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UPhysicsGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (HandleRef && HandleRef->GetGrabbedComponent() && Player)
	{
		FVector ForwardVector = UKismetMathLibrary::GetForwardVector(Player->GetControlRotation());
		FVector Distance = ForwardVector * InteractDistance;
		APlayerController* PlayerController = Player->GetController<APlayerController>();
		FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
		
		/** Update the target location to be in front of the players camera */
		HandleRef->SetTargetLocationAndRotation(CameraLocation + Distance, FRotator::ZeroRotator);
	}
}

void UPhysicsGrabComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Player = GetOwner<APawn>();
	HandleRef = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	ensure(HandleRef);
}

void UPhysicsGrabComponent::Grab()
{
	if (HandleRef->GetGrabbedComponent())
	{
		HandleRef->ReleaseComponent();
	}
	else
	{
		/** Ensure that the pawn owner is valid */
		if (!Player) return;
		APlayerController* PlayerController = Player->GetController<APlayerController>();

		FVector ForwardVector = UKismetMathLibrary::GetForwardVector(Player->GetControlRotation());
		FVector Distance = ForwardVector * InteractDistance;
		FCollisionObjectQueryParams QueryParams = FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody);
		FHitResult Hit;
		FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();

		/** Line trace for physics objects  */
		if (GetWorld()->LineTraceSingleByObjectType(OUT Hit, CameraLocation, CameraLocation + Distance, QueryParams))
		{
			HandleRef->GrabComponentAtLocationWithRotation(Hit.GetComponent(), Hit.BoneName, Hit.GetComponent()->GetComponentLocation(), Hit.GetComponent()->GetComponentRotation());
		}
	}
}
