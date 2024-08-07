// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "ISpudObject.h"
#include "GameFramework/Character.h"
#include "PlayerBase.generated.h"

class IInteraction;
class AInteractable;
class UCameraComponent;
class USpringArmComponent;
class UMatineeCameraShake;
class UInteractorComponent;
class UAttentionComponent;
class UAdvCharacterMovementComponent;
class UPhysicsHandleComponent;
class UPhysicsGrabComponent;
class UPlayerEquipComponent;

UCLASS()
class SPOOKYGAME_API APlayerBase : public ACharacter, public ISpudObject, public ISpudObjectCallback
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	APlayerBase(const FObjectInitializer& ObjectInitializer);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Camera component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;
	//Spring arm component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* ItemSpringArm;
	//Spring arm component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* CameraSpringArm;

	UPROPERTY(EditDefaultsOnly, Category = "Footsteps")
	TSubclassOf<UCameraShakeBase> WalkingScreenShake;

	UPROPERTY(EditDefaultsOnly, Category = "Footsteps|Sounds")
	USoundBase* SoundFootstep;

	/** Rate of footsteps while walking. Rate is proportional to the current speed of the pawn */
	UPROPERTY(EditDefaultsOnly, Category = "Footsteps")
	float WalkFootstepRate = 1.f;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	const UInputAction* JumpAction;

	//Called when player no longer wants to crouch
	void StopCrouch();

	//Called to begin crouching
	void StartCrouch();

	void Landed(const FHitResult& Hit) override;

	virtual void SpudStoreCustomData_Implementation(const USpudState* State, USpudStateCustomData* CustomData) override;

	virtual void SpudRestoreCustomData_Implementation(USpudState* State, USpudStateCustomData* CustomData) override;

	virtual void SpudPreStore_Implementation(const USpudState* State) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//Control player movement forward and backward
	void MovementForward(float AxisValue);

	//Control player movement right and left
	void MovementRight(float AxisValue);

	//Change player move state to sprinting
	void Sprint();
	//Change player move state to running
	void StopSprint();

	//Handle triggering footstep sounds and screen shake
	void TriggerFootstep();

	void FootstepDetection();

	//Handle footstep sound delay
	FTimerHandle FootstepTimer;

	/** Function to cast MovementComponent to custom CharacterMovementComponent */
	UAdvCharacterMovementComponent* GetCharacterMovement() const;
};
