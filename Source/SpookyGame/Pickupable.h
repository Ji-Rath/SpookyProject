// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "UObject/Object.h"
#include "Pickupable.generated.h"

class UPickupableComponent;
/**
 * 
 */
UCLASS()
class SPOOKYGAME_API APickupable : public AInteractable
{
	GENERATED_BODY()

	APickupable(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interactable")
	UPickupableComponent* GetPickupableComponent() const;
};
