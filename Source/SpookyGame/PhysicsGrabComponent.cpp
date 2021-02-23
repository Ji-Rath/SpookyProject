#pragma once

#include "PhysicsGrabComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "TriggerInterface.h"
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
		FVector ForwardVector = UKismetMathLibrary::GetForwardVector(Player->GetControlRotation());
		FVector Distance = ForwardVector * InteractDistance;
		APlayerController* PlayerController = Player->GetController<APlayerController>();
		FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
		
		/** Update the target location to be in front of the players camera */
		HandleRef->SetTargetLocationAndRotation(CameraLocation + Distance, FRotator::ZeroRotator);

		if (PushComponent)
		{
			ForwardVector = UKismetMathLibrary::GetForwardVector(Player->GetActorRotation());
			FHitResult Hit;
			FVector GrabLocation = Player->GetActorLocation() + ForwardVector * InteractDistance;
			FCollisionObjectQueryParams QueryParams = FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic);
			bool bHit = GetWorld()->LineTraceSingleByObjectType(OUT Hit, GrabLocation, GrabLocation - FVector(0.f, 0.f, 100.f), QueryParams);
			DrawDebugLine(GetWorld(), GrabLocation, GrabLocation - FVector(0.f, 0.f, 50.f), FColor::Red, false, 0.5f);
			if (bHit)
			{
				HandleRef->SetTargetLocation(Hit.ImpactPoint);
				DrawDebugPoint(GetWorld(), Hit.ImpactPoint, 10.f, FColor::Red, false, 1.f);
			}
			else
			{
				HandleRef->ReleaseComponent();
			}
			
		}
	}

	/*
	if (IsValid(PushComponent))
	{
		FVector ForwardVector = UKismetMathLibrary::GetForwardVector(Player->GetActorRotation());
		PushComponent->AddForce(ForwardVector * PushForce);
	}
	*/
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
		PushComponent = nullptr;
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
		bool bTraced = GetWorld()->LineTraceSingleByObjectType(OUT Hit, CameraLocation, CameraLocation + Distance, QueryParams);

		/** Ensure there was a trace and that we are not dragging something triggerable */
		GrabMesh = Cast<UStaticMeshComponent>(Hit.GetComponent());
		if (bTraced && GrabMesh && !Hit.GetActor()->Implements<UTriggerInterface>())
		{
			/** Grab object if below threshhold or simply push it */
			if (GrabMesh->GetMass() < GrabThreshhold)
			{
				HandleRef->GrabComponentAtLocationWithRotation(GrabMesh, Hit.BoneName, Hit.GetComponent()->GetComponentLocation(), Hit.GetComponent()->GetComponentRotation());
				OnGrabUpdate.Broadcast(true, nullptr);

				GrabMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
			}
			else
			{
				HandleRef->GrabComponentAtLocation(GrabMesh, Hit.BoneName, Hit.ImpactPoint);
				PushComponent = GrabMesh;

				//GetOwner()->GetWorldTimerManager().SetTimer(PushTimer, this, &UPhysicsGrabComponent::StopPushing, PushDuration, false);
				//ForwardVector = UKismetMathLibrary::GetForwardVector(Player->GetActorRotation());
				//GrabMesh->AddForce(ForwardVector * InitialForce);
 				//PushComponent = GrabMesh;
			}
		}
	}
}

void UPhysicsGrabComponent::StopPushing()
{
	PushComponent = nullptr;
}
