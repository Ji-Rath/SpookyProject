#pragma once

#include "PhysicsGrabComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Interactable.h"
#include "DrawDebugHelpers.h"
#include "AttentionComponent.h"

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
		CurrentGrabDistance = UKismetMathLibrary::FInterpTo(CurrentGrabDistance, GrabRange, DeltaTime, 3.f);
		APlayerController* PlayerController = Player->GetController<APlayerController>();
		FVector ForwardVector = UKismetMathLibrary::GetForwardVector(Player->GetControlRotation());
		FVector Distance = ForwardVector * CurrentGrabDistance;
		FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
		FVector TargetLocation = CameraLocation + Distance;
		
		/** Update the target location to be in front of the players camera */
		HandleRef->SetTargetLocationAndRotation(TargetLocation, FRotator::ZeroRotator);

		/** Release grabbed component if it goes out of range */
		bool bOutsideRange = FVector::Distance(TargetLocation, HandleRef->GetGrabbedComponent()->GetComponentLocation()) > MaxDistance;
		if (bOutsideRange)
		{
			HandleRef->ReleaseComponent();
			OnGrabUpdate.Broadcast(false, nullptr);
		}
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
	UStaticMeshComponent* GrabMesh = Cast<UStaticMeshComponent>(HandleRef->GetGrabbedComponent());
	if (GrabMesh)
	{
		HandleRef->ReleaseComponent();
		OnGrabUpdate.Broadcast(false, nullptr);
		GrabMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	}
	else
	{
		/** Ensure that the pawn owner is valid */
		if (!Player) return;
		APlayerController* PlayerController = Player->GetController<APlayerController>();

		FVector ForwardVector = UKismetMathLibrary::GetForwardVector(Player->GetControlRotation());
		FVector Distance = ForwardVector * InteractDistance;
		FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();

		FCollisionObjectQueryParams QueryParams = FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody);
		FHitResult Hit;
		bool bTraced = GetWorld()->LineTraceSingleByObjectType(OUT Hit, CameraLocation, CameraLocation + Distance, QueryParams);

		/** Ensure there was a trace and that we are not dragging something triggerable */
		GrabMesh = Cast<UStaticMeshComponent>(Hit.GetComponent());
		if (bTraced && GrabMesh)
		{
			/** Grab object if below threshhold or simply push it */
			if (GrabMesh->GetMass() < GrabWeightThreshhold)
			{
				HandleRef->GrabComponentAtLocationWithRotation(GrabMesh, Hit.BoneName, Hit.GetComponent()->GetComponentLocation(), Hit.GetComponent()->GetComponentRotation());
				GrabMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
				OnGrabUpdate.Broadcast(true, nullptr);
				HandleRef->SetTargetLocation(Hit.GetComponent()->GetComponentLocation());
				CurrentGrabDistance = FVector::Distance(Hit.GetComponent()->GetComponentLocation(), CameraLocation);
			}
			else if (!GetWorld()->GetTimerManager().IsTimerActive(PushTimer))
			{
				GrabMesh->AddImpulse(ForwardVector * PushImpulse);
				GetWorld()->GetTimerManager().SetTimer(PushTimer, PushDelay, false);
			}
		}
	}
}
