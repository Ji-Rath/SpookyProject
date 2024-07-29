// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/Interactable.h"
#include "Interaction/Toggleable.h"
#include "Door.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorUnlock);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorInteract);

/**
 * Base class for all door related objects
 */
UCLASS(Abstract, Blueprintable)
class SPOOKYGAME_API ADoor : public AActor, public IInteractable, public IToggleable
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Door|Events")
	FDoorInteract OnDoorInteract;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Door|Events")
	FDoorUnlock OnDoorUnlock;
};
