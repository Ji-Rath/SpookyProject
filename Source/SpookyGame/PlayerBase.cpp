// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"

#include "Kismet/KismetMathLibrary.h"
#include "TriggerInterface.h"
#include "InteractableBase.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "PlayerControllerBase.h"
#include "MainUIBase.h"
#include "TriggerComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
APlayerBase::APlayerBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlayerBase::BeginPlay()
{
	Super::BeginPlay();

	InitialCapsuleHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void APlayerBase::MovementForward(float AxisValue)
{
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(GetControlRotation());
	AddMovementInput(ForwardVector, AxisValue);

	if(!GetWorldTimerManager().IsTimerActive(FootstepTimer))
		GetWorldTimerManager().SetTimer(FootstepTimer, this, &APlayerBase::HandleFootsteps, 1.f);
}

void APlayerBase::MovementRight(float AxisValue)
{
	FVector RightVector = UKismetMathLibrary::GetRightVector(GetControlRotation());
	AddMovementInput(RightVector, AxisValue);

	if (!GetWorldTimerManager().IsTimerActive(FootstepTimer))
		GetWorldTimerManager().SetTimer(FootstepTimer, this, &APlayerBase::HandleFootsteps, 1.f);
}

void APlayerBase::HoverInteraction(float DeltaTime)
{
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(GetControlRotation());
	FVector Distance = ForwardVector * InteractDistance;
	FCollisionQueryParams QueryParams = FCollisionQueryParams(FName(TEXT("Interaction Actor")), false, this);
	FHitResult Hit;
	FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
	APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(GetController());

	//Line trace for interactable objects
	if (GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, CameraLocation + Distance, ECC_Visibility, QueryParams))
	{
		if (ensure(Hit.GetActor()) && Cast<AInteractableBase>(Hit.GetActor()))
		{
			InteractHover = Cast<AInteractableBase>(Hit.GetActor());
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

void APlayerBase::Interact()
{
	if (IsValid(InteractHover) && InteractHover->bCanInteract)
	{
		UTriggerComponent* TriggerComponent = InteractHover->FindComponentByClass<UTriggerComponent>();
		if (TriggerComponent != nullptr)
		{
			TriggerComponent->TriggerActors();
		}

	}
}

// Called every frame
void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Get actors in front of player that are interactable
	HoverInteraction(DeltaTime);

	HandleCrouching(DeltaTime);
}

// Called to bind functionality to input
void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerBase::MovementForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerBase::MovementRight);

	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerBase::Interact);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerBase::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APlayerBase::StopCrouch);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerBase::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerBase::StopSprint);

}

void APlayerBase::HandleCrouching(float DeltaTime)
{
	UCharacterMovementComponent* MoveComponent = GetCharacterMovement();
	FHitResult Hit;
	float SmoothCrouch = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	if (bIsCrouching && GetMovementComponent()->IsMovingOnGround())
	{
		SmoothCrouch = FMath::FInterpTo(SmoothCrouch, MoveComponent->CrouchedHalfHeight, DeltaTime, CrouchSpeed);
		MoveComponent->MaxWalkSpeed = MoveComponent->MaxWalkSpeedCrouched;

		GetCapsuleComponent()->SetCapsuleHalfHeight(SmoothCrouch);

	}
	else if (GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() != InitialCapsuleHeight && !GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), GetActorLocation() + (GetActorLocation().UpVector * InitialCapsuleHeight), ECC_Visibility, FCollisionQueryParams(NAME_None, false, this)))
	{
		SmoothCrouch = FMath::FInterpTo(SmoothCrouch, InitialCapsuleHeight, DeltaTime, CrouchSpeed);

		GetCapsuleComponent()->SetCapsuleHalfHeight(SmoothCrouch);

		if(bIsSprinting)
			MoveComponent->MaxWalkSpeed = SprintSpeed;
		else
			MoveComponent->MaxWalkSpeed = WalkSpeed;
	}
}

void APlayerBase::HandleFootsteps()
{
	UE_LOG(LogTemp, Log, TEXT("Test"));
}

