// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "TriggerInterface.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/Controller.h"
#include "PlayerControllerBase.h"
#include "PlayerInteractComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AttentionComponent.h"
#include "AdvCharacterMovementComponent.h"
#include "PhysicsGrabComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "PlayerEquipComponent.h"

// Sets default values
APlayerBase::APlayerBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UAdvCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
	CameraSpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraSpringArm);

	ItemSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	ItemSpringArm->SetupAttachment(Camera);

	PlayerInteract = CreateDefaultSubobject<UPlayerInteractComponent>(TEXT("Player Interact"));
	AttentionComp = CreateDefaultSubobject<UAttentionComponent>(TEXT("Attention Component"));
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("Physics Handle"));
	PhysicsGrab = CreateDefaultSubobject<UPhysicsGrabComponent>(TEXT("Physics Grab"));
	PlayerEquip = CreateDefaultSubobject<UPlayerEquipComponent>(TEXT("Player Equip"));
}

// Called when the game starts or when spawned
void APlayerBase::BeginPlay()
{
	Super::BeginPlay();

}

void APlayerBase::MovementForward(float AxisValue)
{
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(GetActorRotation());
	AddMovementInput(ForwardVector, AxisValue);

	FootstepDetection();
}

void APlayerBase::MovementRight(float AxisValue)
{
	FVector RightVector = UKismetMathLibrary::GetRightVector(GetActorRotation());
	AddMovementInput(RightVector, AxisValue);

	FootstepDetection();
}

void APlayerBase::Sprint()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Custom, CUSTOM_Sprint);
	bIsSprinting = true;
}

// Called every frame
void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerBase::MovementForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerBase::MovementRight);

	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, PlayerInteract, &UPlayerInteractComponent::Interact);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, PhysicsGrab, &UPhysicsGrabComponent::Grab);
	PlayerInputComponent->BindAction("Interact", IE_Released, PhysicsGrab, &UPhysicsGrabComponent::Grab);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerBase::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APlayerBase::StopCrouch);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerBase::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerBase::StopSprint);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerBase::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerBase::StopJumping);

	PlayerInputComponent->BindAction("Inventory", IE_Pressed, GetController<APlayerControllerBase>(), &APlayerControllerBase::ToggleInventory);

	PlayerInputComponent->BindAction("DropItem", IE_Pressed, PlayerEquip, &UPlayerEquipComponent::DropEquippedItem);
}

void APlayerBase::StopSprint()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	bIsSprinting = false;
}

void APlayerBase::TriggerFootstep()
{
	if(!ensure(WalkingScreenShake != nullptr || SoundFootstep != nullptr)) return;

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundFootstep, GetActorLocation());
	APlayerController* PlayerController = GetController<APlayerController>();
	PlayerController->PlayerCameraManager->StartCameraShake(WalkingScreenShake);
}

void APlayerBase::FootstepDetection()
{
	//Ensure player is moving
	bool bWantsToMove = GetCharacterMovement()->IsWalking() || GetCharacterMovement()->CustomMovementMode == CUSTOM_Sprint;
	if (bWantsToMove && GetVelocity().Size() > 0)
	{
		if (!GetWorldTimerManager().IsTimerActive(FootstepTimer))
		{
			float Timer = WalkFootstepRate;
			if (GetCharacterMovement()->IsSprinting()) Timer = RunFootstepRate;
			if (GetCharacterMovement()->IsCrouching()) Timer = CrouchFootstepRate;
			GetWorldTimerManager().SetTimer(FootstepTimer, this, &APlayerBase::TriggerFootstep, Timer);
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(FootstepTimer);
	}
}

void APlayerBase::OnEndCrouch(float HeightAdjust, float ScaledHeightAdjust)
{
	Super::OnEndCrouch(HeightAdjust, HeightAdjust);

	if (bIsSprinting)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Custom, CUSTOM_Sprint);
	}
}

UAdvCharacterMovementComponent* APlayerBase::GetCharacterMovement()
{
	return Cast<UAdvCharacterMovementComponent>(Super::GetCharacterMovement());
}

void APlayerBase::StopCrouch()
{
	GetCharacterMovement()->bWantsToCrouch = false;
}

void APlayerBase::StartCrouch()
{
	GetCharacterMovement()->bWantsToCrouch = true;
}
