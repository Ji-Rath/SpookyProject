// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"

#include "AdvCharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/Controller.h"
#include "PlayerControllerBase.h"
#include "Interaction/PlayerInteractComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AttentionComponent.h"
#include "AdvCharacterMovementComponent.h"
#include "Interaction/PhysicsGrabComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Inventory/PlayerEquipComponent.h"

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
}

// Called when the game starts or when spawned
void APlayerBase::BeginPlay()
{
	Super::BeginPlay();

}

void APlayerBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	
	TriggerFootstep();
	GetWorldTimerManager().ClearTimer(FootstepTimer);
}

void APlayerBase::SpudStoreCustomData_Implementation(const USpudState* State, USpudStateCustomData* CustomData)
{
	ISpudObjectCallback::SpudStoreCustomData_Implementation(State, CustomData);

	if (auto* InventoryComponent = FindComponentByClass<UInventoryComponent>())
	{
		TArray<FInventoryContents> Inventory;
		InventoryComponent->GetInventory(Inventory);
		CustomData->Write<TArray < FInventoryContents > >(Inventory);
		//UE_LOG(LogTemp, Error, TEXT("SAVED INVENTORY DATA!"));
	}
}

void APlayerBase::SpudRestoreCustomData_Implementation(USpudState* State, USpudStateCustomData* CustomData)
{
	ISpudObjectCallback::SpudRestoreCustomData_Implementation(State, CustomData);
	
	if (auto* InventoryComponent = FindComponentByClass<UInventoryComponent>())
	{
		TArray<FInventoryContents> Inventory;
		CustomData->Read<TArray < FInventoryContents > >(Inventory);
		InventoryComponent->SetInventory(Inventory);
		//UE_LOG(LogTemp, Error, TEXT("LOADED INVENTORY DATA!"));
	}
}

void APlayerBase::SpudPreStore_Implementation(const USpudState* State)
{
	ISpudObjectCallback::SpudPreStore_Implementation(State);

	if (auto* EquipComp = FindComponentByClass<UPlayerEquipComponent>())
	{
		EquipComp->UnequipItem();
	}
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

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerBase::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APlayerBase::StopCrouch);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, GetCharacterMovement(), &UAdvCharacterMovementComponent::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, GetCharacterMovement(), &UAdvCharacterMovementComponent::StopSprint);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Inventory", IE_Pressed, GetController<APlayerControllerBase>(), &APlayerControllerBase::ToggleInventory);

	PlayerInputComponent->BindAction("Escape", IE_Pressed, GetController<APlayerControllerBase>(), &APlayerControllerBase::PauseGame);
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
	if (!GetCharacterMovement()) { return; }

	//Ensure player is moving
	bool bWantsToMove = !GetCharacterMovement()->IsFalling() && (GetCharacterMovement()->IsMovingOnGround());
	if (bWantsToMove && GetVelocity().Size() > 0.f)
	{
		if (!GetWorldTimerManager().IsTimerActive(FootstepTimer) && !GetWorldTimerManager().IsTimerPaused(FootstepTimer))
		{
			const float VelocityRatio = GetVelocity().Size() / GetCharacterMovement()->MaxWalkSpeed;
			float Timer = FMath::Clamp(WalkFootstepRate / VelocityRatio, 0.1f, 0.8f);
			GetWorldTimerManager().SetTimer(FootstepTimer, this, &APlayerBase::TriggerFootstep, Timer);
		}
		else if (GetWorldTimerManager().IsTimerPaused(FootstepTimer))
		{
			GetWorldTimerManager().UnPauseTimer(FootstepTimer);
		}
	}
	else
	{
		GetWorldTimerManager().PauseTimer(FootstepTimer);
	}
}

UAdvCharacterMovementComponent* APlayerBase::GetCharacterMovement() const
{
	return Cast<UAdvCharacterMovementComponent>(Super::GetCharacterMovement());
}

void APlayerBase::StopCrouch()
{
	UnCrouch();
}

void APlayerBase::StartCrouch()
{
	Crouch();
}
