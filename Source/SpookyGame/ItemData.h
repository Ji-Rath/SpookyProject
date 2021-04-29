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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxStack;

	UPROPERTY(EditAnywhere, meta = (AllowedClasses = "Pickupable"), BlueprintReadOnly)
	TSubclassOf<AActor> ActorClass;

	/** Compare classes when comparing ItemData */
	bool operator==(const UItemData& OtherItem) const
	{
		return GetClass() == OtherItem.GetClass();
	}

	UItemData() {
		Name = FText::FromString("NoName");
		Description = FText::FromString("NoDesc");
		MaxStack = 1;
	}
};
