// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemData.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryChange, bool, bAdded); 

USTRUCT()
struct FInventoryContents
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	UItemData* ItemData;
	UPROPERTY(EditAnywhere)
	int Count;
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
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	FInventoryChange OnInventoryChange;

	/** Equip the item that is in the slot */
	UFUNCTION(BlueprintCallable)
	void EquipSlot(int Slot);

	/** Unequip any currently equipped items */
	UFUNCTION(BlueprintCallable)
	void UnequipItem();

	/** Drop an item from selected slot */
	UFUNCTION(BlueprintCallable)
	void DropItemFromSlot(int Slot, int Count);

	/** Remove an item from current inventory */
	UFUNCTION(BlueprintCallable)
	void RemoveFromInventory(int ItemSlot, const int Count);

	/**
	 * Find the first slot containing Item
	 * @param Item - Item to find
	 * @return Slot with containing item
	*/
	UFUNCTION(BlueprintCallable)
	int FindItemSlot(UItemData* Item);

	/**
	 * Returns the currently equipped slot
	 * @warning Will return -1 if there is no slot equipped
	 */
	UFUNCTION(BlueprintCallable)
	int GetEquippedSlot() const;

	/**
	 * Attempt to add an item to the inventory
	 * @param Item - Item to add
	 * @param Count - Amount of item
	 * @return Whether the item could be added to the inventory
	*/
	UFUNCTION(BlueprintCallable)
	bool AddToInventory(UItemData* Item, const int Count);

	TArray<FInventoryContents> GetInventory() const;
	
private:
	UPROPERTY(EditAnywhere)
	TArray<FInventoryContents> Inventory;

	UPROPERTY(EditDefaultsOnly)
	int InventorySize;

	UPROPERTY(EditAnywhere, meta = (UseComponentPicker))
	FComponentReference ItemAttachParent;

	int EquippedSlot = -1;
};
