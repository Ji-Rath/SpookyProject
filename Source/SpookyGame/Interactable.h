// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteract, AActor *, Interactor);

class UItemData;

UCLASS()
class SPOOKYGAME_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable();

	void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/** Implementation of interaction, call Interact() to trigger function */
	UFUNCTION(BlueprintNativeEvent)
	void OnInteract(AActor* Interactor);

	/**
	 * Called to interact with interactable
	 * @param Interactor The actor that is trying to interact with the actor
	 * @return bool Whether the interaction was successful
	*/
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool Interact(AActor* Interactor);

	/** Returns whether the interactable can be interacted with */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool CanInteract(AActor* Interactor) const;

	/** Change the ability of the actor to be interacted with */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetInteractable(bool bIsInteractable);

	/** Returns the item data */
	UFUNCTION(BlueprintCallable, Category = "Interaction|Data")
	UItemData* GetItemData() const;

	/** Returns the name of the interactable */
	UFUNCTION(BlueprintCallable, Category = "Interaction|Data")
	FText GetName() const;

	UPROPERTY(BlueprintAssignable)
	FInteract OnInteracted;

protected:
	/** Determine whether the player can interact with the actor */
	UPROPERTY(EditAnywhere, Category = "Interaction")
	bool bCanInteract = true;

	/** Amount of times the actor can be interacted, -1 for no limit */
	UPROPERTY(EditAnywhere, Category = "Interaction")
	int InteractAmount = -1;

	/** Determines whether the interactable will use a DataAsset for primary info */
	UPROPERTY(EditAnywhere, Category = "Interaction|Data")
	bool bUseData = false;

	/**
	 * Name of interactable
	 * @warning overrided by DataAsset name if bUseData is true
	 */
	UPROPERTY(EditAnywhere, Category = "Interaction|Data", meta = (EditCondition = "!bUseData"))
	FText Name = FText::FromString("NoName");

	UPROPERTY(EditAnywhere, Category = "Interaction|Data", meta = (EditCondition = "bUseData"))
	UItemData* ItemData = nullptr;

private:
	int InteractCount = 0;
};
