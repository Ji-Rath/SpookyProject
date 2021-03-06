// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerBase.generated.h"

class ITriggerInterface;
class AInteractableBase;
class UCameraComponent;

UCLASS()
class SPOOKYGAME_API APlayerBase : public ACharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	APlayerBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Distance that the player can interact with objects
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float InteractDistance = 500.f;

	//Camera component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;

	//Speed to crouch and uncrouch
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float CrouchSpeed = 5.f;

	//Player walking speed (Copied from CharacterMovement Component)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float WalkSpeed = 250.f;

	//Player running speed
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed = 350.f;

	//Store interact actor that the player is currently looking at
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AInteractableBase* InteractHover = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	//Control player movement forward and backward
	void MovementForward(float AxisValue);

	//Control player movement right and left
	void MovementRight(float AxisValue);

	void HoverInteraction(float DeltaTime);

	void Interact();

	//Change player move state to sprinting
	void Sprint() { GetCharacterMovement()->MaxWalkSpeed = SprintSpeed; bIsSprinting = true; }

	//Change player move state to running
	void StopSprint() { GetCharacterMovement()->MaxWalkSpeed = WalkSpeed; bIsSprinting = false; }

	//Called to begin crouching
	void StartCrouch() { bIsCrouching = true; }

	//Called when player no longer wants to crouch
	void StopCrouch() { bIsCrouching = false; }

	//Called every tick to handle smooth crouching
	void HandleCrouching(float DeltaTime);

	//Store initial capsulehalfheight
	float InitialCapsuleHeight;

	//Handle triggering footstep sounds and screen shake
	UFUNCTION()
	void HandleFootsteps();

	//Handle footstep sound delay
	FTimerHandle FootstepTimer;

	//Store interact actor that player is interacting with
	AInteractableBase* CurrentInteractActor = nullptr;

	//When true, player wants to crouch
	bool bIsCrouching;

	//When true, player wants to sprint
	bool bIsSprinting;
};
