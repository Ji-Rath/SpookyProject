#include "PlayerPushComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interaction/ToggleInteractable.h"
#include "DrawDebugHelpers.h"


UPlayerPushComponent::UPlayerPushComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
		// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UPlayerPushComponent::BeginPlay()
{
	Super::BeginPlay();
	
	/** Get Character Movement Component from owner */
	CharacterMoveComp = GetOwner()->FindComponentByClass<UCharacterMovementComponent>();
	ensureMsgf(CharacterMoveComp, TEXT("No Character Movement Component found for %s"), *(GetOwner()->GetName()));

	/** Get Physics Handle Component from owner */
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	ensureMsgf(CharacterMoveComp, TEXT("No Physics Handle Component found for %s"), *(GetOwner()->GetName()));
}

void UPlayerPushComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bPushingObject && PhysicsHandle->GetGrabbedComponent())
	{
		FVector OwnerLocation = GetOwner()->GetActorLocation();
		FVector TargetLocation = TargetOffset + OwnerLocation;

		/** Ensure player does not get too far from TargetLocation */
		FHitResult OutHit;
		GetWorld()->LineTraceSingleByChannel(OutHit, OwnerLocation, OwnerLocation + (UKismetMathLibrary::GetForwardVector(TargetRotation) * MaxObjectDistance), ECC_Visibility);

		AToggleInteractable* PushBox = Cast<AToggleInteractable>(PhysicsHandle->GetGrabbedComponent()->GetOwner());
		/* Stop pushing if the object is no longer infront of the player */
		if (OutHit.GetComponent() != PhysicsHandle->GetGrabbedComponent())
		{
			PushBox->SetState(GetOwner(), false);
		}

		/* Stop pushing if the object is no longer infront of the player */
		FHitResult BoxHit;
		UStaticMeshComponent* BoxMesh = Cast<UStaticMeshComponent>(PhysicsHandle->GetGrabbedComponent());
		if (BoxMesh && !GetWorld()->LineTraceSingleByChannel(BoxHit, BoxMesh->GetCenterOfMass(), BoxMesh->GetCenterOfMass() + (100.f * FVector(0.f, 0.f, -1.f)), ECC_Visibility))
		{
			PushBox->SetState(GetOwner(), false);
		}

		/* Push object away if its too close, fixes slow movement while pushing */
		if (FVector::Distance(OutHit.ImpactPoint, OwnerLocation) < MinObjectDistance)
		{
			TargetLocation += UKismetMathLibrary::GetForwardVector(TargetRotation) * (MinObjectDistance - FVector::Distance(OutHit.ImpactPoint, OwnerLocation));
			FVector CurrentLocation;
			FRotator CurrentRotation;
			PhysicsHandle->GetTargetLocationAndRotation(CurrentLocation, CurrentRotation);
			TargetLocation = UKismetMathLibrary::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, 5.f);
		}

		/** Update physics handle target location */
		PhysicsHandle->SetTargetLocation(TargetLocation);
		TargetRotation = UKismetMathLibrary::MakeRotFromX(OutHit.ImpactNormal * -1.f);
	}
}

void UPlayerPushComponent::PushObject(UPrimitiveComponent* PushComponent)
{
	if (!CharacterMoveComp || !PhysicsHandle || !PushComponent) { return; }
	
	bPushingObject = true;

	CharacterMoveComp->MaxWalkSpeed = PushMaxSpeed;
	CharacterMoveComp->bOrientRotationToMovement = false;

	/* Draw a line from owner to pushed object center of mass */
	FHitResult OutHit;
	GetWorld()->LineTraceSingleByChannel(OUT OutHit, GetOwner()->GetActorLocation(), PushComponent->GetCenterOfMass(), ECC_Visibility);

	/* Sets rotation to face pushed object relative to player */
	TargetRotation = UKismetMathLibrary::MakeRotFromX(OutHit.ImpactNormal * -1.f);

	/* Pushed object offset relative to player */
	TargetOffset = OutHit.ImpactPoint - GetOwner()->GetActorLocation();

	/* Grab object */
	PhysicsHandle->GrabComponentAtLocationWithRotation(PushComponent, FName(), OutHit.ImpactPoint, FRotator(0.f,0.f,PushComponent->GetComponentRotation().Yaw));

	OnPushObject.Broadcast(true);
}

void UPlayerPushComponent::StopPushing()
{
	if (!CharacterMoveComp || !PhysicsHandle) { return; }

	bPushingObject = false;

	CharacterMoveComp->MaxWalkSpeed = 400.f;
	CharacterMoveComp->bOrientRotationToMovement = true;

	PhysicsHandle->ReleaseComponent();

	OnPushObject.Broadcast(false);
}

bool UPlayerPushComponent::IsPushingObject()
{
	return bPushingObject;
}
