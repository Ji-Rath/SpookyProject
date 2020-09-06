// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerBase.generated.h"

class ITriggerInterface;
class AInteractableBase;

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
	
	//Store interact actor that the player is currently looking at
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AInteractableBase* InteractHover = nullptr;

	//Store interact actor that player is interacting with
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AInteractableBase* CurrentInteractActor = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	//Control player movement forward and backward
	void MovementForward(float AxisValue);

	//Control player movement right and left
	void MovementRight(float AxisValue);

	void HoverInteraction(float DeltaTime);
};
