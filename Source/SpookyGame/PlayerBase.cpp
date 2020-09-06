// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"

#include "Kismet/KismetMathLibrary.h"
#include "TriggerInterface.h"
#include "InteractableBase.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"

// Sets default values
APlayerBase::APlayerBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerBase::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerBase::MovementForward(float AxisValue)
{
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(GetControlRotation());
	AddMovementInput(ForwardVector, AxisValue);
}

void APlayerBase::MovementRight(float AxisValue)
{
	FVector RightVector = UKismetMathLibrary::GetRightVector(GetControlRotation());
	AddMovementInput(RightVector, AxisValue);
}

void APlayerBase::HoverInteraction(float DeltaTime)
{
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(GetControlRotation());
	FVector Distance = ForwardVector * InteractDistance;
	FCollisionQueryParams QueryParams = FCollisionQueryParams(FName(TEXT("Interaction Actor")), false, this);
	FHitResult Hit;
	FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();

	if (GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, CameraLocation + Distance, ECC_Visibility, QueryParams))
	{
		if (ensure(Hit.GetActor()) && Cast<AInteractableBase>(Hit.GetActor()))
		{
			InteractHover = Cast<AInteractableBase>(Hit.GetActor());

			APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(GetController());
			PlayerController->MainUI->PlayInteractAnim(EUMGSequencePlayMode::Forward);
		}
		else
		{
			InteractHover = nullptr;
		}
	}
	else
	{
		InteractHover = nullptr;
	}
}

// Called every frame
void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HoverInteraction(DeltaTime);
}

// Called to bind functionality to input
void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerBase::MovementForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerBase::MovementRight);

	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &APawn::AddControllerYawInput);

}

