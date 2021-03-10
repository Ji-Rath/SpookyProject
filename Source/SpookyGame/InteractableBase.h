// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerInterface.h"
#include "InteractableBase.generated.h"

class UItemData;

UCLASS()
class SPOOKYGAME_API AInteractableBase : public AActor, public ITriggerInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableBase();

	void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	/**
	 * Toggle the status of the interactable
	 * @warning Will not toggle more than once if bOneTimeInteraction is true
	 */
	UFUNCTION(BlueprintCallable)
	void ToggleOnStatus();

	/** Returns whether the interactable can be interacted with */
	bool CanTrigger() const;
	bool CanTrigger_Implementation() const override;

	/** Returns whether the player can interact with the actor */
	bool CanPlayerTrigger() const;
	bool CanPlayerTrigger_Implementation() const override;

	/** Returns the name of the interactable */
	FText GetName() const;
	FText GetName_Implementation() const override;

	/** Change the ability of the actor to be interacted with */
	UFUNCTION(BlueprintCallable)
	void SetInteractable(bool bIsInteractable);

	/** Returns the item data */
	UFUNCTION(BlueprintCallable)
	UItemData* GetItemData() const;

	UFUNCTION(BlueprintCallable)
	bool GetCurrentState() const;

protected:
	//Determine whether anything can interact with the actor
	UPROPERTY(EditAnywhere, Category = "Interaction")
	bool bCanInteract = true;

	//Determine whether the player can interact with the actor
	UPROPERTY(EditAnywhere, Category = "Interaction")
	bool bPlayerInteract = false;

	/** Determines whether the interactable will use a DataAsset for primary info */
	UPROPERTY(EditAnywhere, Category = "Data")
	bool bUseData = false;

	/**
	 * Name of interactable
	 * @warning overrided by DataAsset name if bUseData is true
	 */
	UPROPERTY(EditAnywhere, Category = "Data", meta = (EditCondition = "!bUseData"))
	FText Name;

	/** Whether the actor can only be used once */
	UPROPERTY(EditAnywhere, Category = "Interaction")
	bool bOneTimeInteraction = false;

	/** Sets the initial state of the interactable */
	UPROPERTY(EditAnywhere, Category = "Interaction")
	bool bIsOn = false;

	UPROPERTY(EditAnywhere, Category = "Data", meta = (EditCondition = "bUseData"))
	UItemData* ItemData = nullptr;
};
