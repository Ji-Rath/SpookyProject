// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInteractComponent.generated.h"

class IInteraction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateInteract, bool, bInteractable, TScriptInterface<IInteraction>, Actor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteract, AActor*, Interactable);

/**
 * Allows the player to interact with interactables, executing functionality based on what was interacted with
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPOOKYGAME_API UPlayerInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerInteractComponent();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Delegate called when there is a change in InteractHover
	UPROPERTY()
	FUpdateInteract OnUpdateInteract;

	UPROPERTY()
	FInteract OnInteract;

	/** Called when the player wants to interact with the currently viewed interactable */
	UFUNCTION(BlueprintCallable)
	void Interact();

private:
	void HoverInteraction(float DeltaTime);

	//Store interact actor that the player is currently looking at
	UPROPERTY(VisibleAnywhere)
	AActor* InteractHover = nullptr;

	//Distance that the player can interact with objects
	UPROPERTY(EditDefaultsOnly)
	float InteractDistance = 500.f;
};
