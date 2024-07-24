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

	virtual void BeginPlay() override;

	virtual void SpudStoreCustomData_Implementation(const USpudState* State, USpudStateCustomData* CustomData) override;

	virtual void SpudRestoreCustomData_Implementation(USpudState* State, USpudStateCustomData* CustomData) override;

	virtual void SpudPostRestore_Implementation(const USpudState* State) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interactable")
	UPickupableComponent* GetPickupableComponent() const;
	
	UPROPERTY()
	FGuid SpudGuid;
};
