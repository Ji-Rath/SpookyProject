// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TriggerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTriggerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPOOKYGAME_API ITriggerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	//Called from a trigger component to change the state of the actor
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnTrigger(const AActor* Interactor);
};
