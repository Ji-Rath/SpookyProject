// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerBase.generated.h"

class ITriggerInterface;
class AInteractableBase;
class UCameraComponent;
class USpringArmComponent;
class USpotLightComponent;
class UMatineeCameraShake;
class UPlayerInteractComponent;
class UAttentionComponent;
class UAdvCharacterMovementComponent;

UCLASS()
class SPOOKYGAME_API APlayerBase : public ACharacter
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
	USpringArmComponent* LightSpringArm;
	//Spring arm component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* CameraSpringArm;
	//Flashlight component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpotLightComponent* Flashlight;
	//Player interact component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPlayerInteractComponent* PlayerInteract;
	//Player attention component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAttentionComponent* AttentionComp;

	UPROPERTY(EditDefaultsOnly, Category = "Footsteps")
	TSubclassOf<UMatineeCameraShake> WalkingScreenShake;

	UPROPERTY(EditDefaultsOnly, Category = "Footsteps|Sounds")
	USoundBase* SoundFootstep;

	UPROPERTY(EditDefaultsOnly, Category = "Footsteps")
	float WalkFootstepRate = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = "Footsteps")
	float RunFootstepRate = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = "Footsteps")
	float CrouchFootstepRate = 1.f;

	//Called when player no longer wants to crouch
	void StopCrouch();

	//Called to begin crouching
	void StartCrouch();

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

	//When true, player wants to sprint
	bool bIsSprinting;

	void OnEndCrouch(float HeightAdjust, float ScaledHeightAdjust) override;

	/** Function to cast MovementComponent to custom CharacterMovementComponent */
	UAdvCharacterMovementComponent* GetCharacterMovement();
};
