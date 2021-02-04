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
#include "PlayerInteractComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AttentionComponent.h"

// Sets default values
APlayerBase::APlayerBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
	CameraSpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraSpringArm);

	LightSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	LightSpringArm->SetupAttachment(Camera);

	Flashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
	Flashlight->SetupAttachment(LightSpringArm);

	PlayerInteract = CreateDefaultSubobject<UPlayerInteractComponent>(TEXT("Player Interact"));

	AttentionComp = CreateDefaultSubobject<UAttentionComponent>(TEXT("Attention Component"));

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
	}
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

	FootstepDetection();
}

void APlayerBase::MovementRight(float AxisValue)
{
	FVector RightVector = UKismetMathLibrary::GetRightVector(GetControlRotation());
	AddMovementInput(RightVector, AxisValue);

	FootstepDetection();
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

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerBase::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APlayerBase::StopCrouch);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerBase::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerBase::StopSprint);

}

void APlayerBase::TriggerFootstep()
{
	if(!ensure(WalkingScreenShake != nullptr || SoundFootstep != nullptr)) return;

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundFootstep, GetActorLocation());
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->PlayerCameraManager->StartCameraShake(WalkingScreenShake);
}

void APlayerBase::FootstepDetection()
{
	//Ensure player is moving
	if (GetCharacterMovement()->IsWalking() && GetVelocity().Size() > 0)
	{
		if (!GetWorldTimerManager().IsTimerActive(FootstepTimer))
		{
			float Timer = WalkFootstepRate;
			if (bIsSprinting) Timer = RunFootstepRate;
			//if (bIsCrouching) Timer = CrouchFootstepRate;
			GetWorldTimerManager().SetTimer(FootstepTimer, this, &APlayerBase::TriggerFootstep, Timer);
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(FootstepTimer);
	}
}

void APlayerBase::StopCrouch()
{
	GetCharacterMovement()->UnCrouch(true);
}

void APlayerBase::StartCrouch()
{
	GetCharacterMovement()->Crouch(true);
}
