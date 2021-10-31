// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInteractComponent.generated.h"

class AInteractable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateInteract, bool, bInteractable, AInteractable*, Interactable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerInteract, AInteractable*, Interactable);

/**
 * Allows the player to interact with interactables, executing functionality based on what was interacted with
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INTERACTIONSYSTEM_API UPlayerInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerInteractComponent();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Delegate called when there is a change in InteractHover
	UPROPERTY(BlueprintAssignable)
	FUpdateInteract OnUpdateInteract;

	UPROPERTY(BlueprintAssignable)
	FPlayerInteract OnInteract;

	/**
	 * Called when the player wants to interact with the currently viewed interactable
	 * @param returns interacted actor
	 * @return whether the interaction was successful
	*/
	UFUNCTION(BlueprintCallable)	
	AInteractable* Interact(bool& bSuccess);

	/**
	 * BindAction friendly version of Interact()
	 * @return void
	*/
	UFUNCTION()
	void InteractAction();

protected:
	//Store interact actor that the player is currently looking at
	UPROPERTY(VisibleAnywhere)
	AInteractable* InteractHover = nullptr;

	//Distance that the player can interact with objects
	UPROPERTY(EditDefaultsOnly)
	float InteractDistance = 500.f;
	
	UFUNCTION()
	virtual void HoverInteraction(float DeltaTime);
};
