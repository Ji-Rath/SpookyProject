// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInteractComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerControllerBase.h"
#include "TriggerComponent.h"
#include "InteractableBase.h"
#include "MainUIBase.h"


// Sets default values for this component's properties
UPlayerInteractComponent::UPlayerInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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
	APawn* Player = Cast<APawn>(GetOwner());
	APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(Player->GetController());

	if (!PlayerController) return;
	if (!PlayerController->MainUI) return;

	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(Player->GetControlRotation());
	FVector Distance = ForwardVector * InteractDistance;
	FCollisionQueryParams QueryParams = FCollisionQueryParams(FName(TEXT("Interaction Actor")), false, Player);
	FHitResult Hit;
	FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();

	//Line trace for interactable objects
	if (GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, CameraLocation + Distance, ECC_Visibility, QueryParams))
	{
		InteractHover = Cast<AInteractableBase>(Hit.GetActor());

		//Set visibility of interact UI
		if (IsValid(InteractHover) && InteractHover->bPlayerInteract)
		{
			PlayerController->MainUI->PlayInteractAnim(EUMGSequencePlayMode::Forward);
		}
		else
		{
			InteractHover = nullptr;
			PlayerController->MainUI->PlayInteractAnim(EUMGSequencePlayMode::Reverse);
		}
	}
	else
	{
		InteractHover = nullptr;
		PlayerController->MainUI->PlayInteractAnim(EUMGSequencePlayMode::Reverse);
	}
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