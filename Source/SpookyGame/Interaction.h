// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interaction.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteraction : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPOOKYGAME_API IInteraction
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	/** Called from a trigger component to change the state of the actor */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnInteract(const AActor* Interactor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanInteract() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanPlayerInteract() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText GetName() const;
};