// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"

/**
 * Data Asset that stores essential values for an interactable, more specifically a pickupable
 */
UCLASS()
class SPOOKYGAME_API UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(EditAnywhere)
	int MaxStack;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AInteractableBase> ActorClass;
};
