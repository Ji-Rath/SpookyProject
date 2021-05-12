// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemData.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryChange, bool, bAdded);

USTRUCT(Blueprintable)
struct FInventoryContents
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UItemData* ItemData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Count;

	FInventoryContents()
	{
		ItemData = nullptr;
		Count = 0;
	}

	FInventoryContents(UItemData* Data, int Amount)
	{
		ItemData = Data;
		Count = Amount;
	}

	bool operator==(const FInventoryContents& OtherSlot) const
	{
		return ItemData == OtherSlot.ItemData;
	}
};

/**
 * Inventory system that stores inventory values and allows manipulation of contents
 * Includes equip/unequip system
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPOOKYGAME_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UPROPERTY()
	FInventoryChange OnInventoryChange;

	/** Drop an item from selected slot */
	UFUNCTION(BlueprintCallable)
	void DropItem(const UItemData* Item, const int Count = 1);

	/** Remove an item from current inventory */
	UFUNCTION(BlueprintCallable)
	void RemoveFromInventory(const UItemData* Item, const int Count = 1);

	/**
	 * Find the first slot containing Item
	 * @param Item - Item to find
	 * @return Slot with containing item
	*/
	UFUNCTION(BlueprintCallable)
	int FindItemSlot(const UItemData* Item) const;

	/**
	 * Find the first slot containing Item
	 * @param Item - Item to find
	 * @return Slot with containing item
	*/
	UFUNCTION(BlueprintCallable)
	UItemData* FindItem(const int Index) const;

	/**
	 * Attempt to add an item to the inventory
	 * @param Item - Item to add
	 * @param Count - Amount of item
	 * @return Whether the item could be added to the inventory
	*/
	UFUNCTION(BlueprintCallable)
	bool AddToInventory(UItemData* Item, const int Count);

	void GetInventory(TArray<FInventoryContents>& OutInventory) const;
	
private:
	UPROPERTY(EditAnywhere)
	TArray<FInventoryContents> Inventory;

	UPROPERTY(EditDefaultsOnly)
	int InventorySize;
};
