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

	UPROPERTY(EditAnywhere)
	TArray<FInventoryContents> Inventory;

	UPROPERTY(EditDefaultsOnly)
	int InventorySize;

	UPROPERTY()
	FInventoryChange OnInventoryChange;

	UPROPERTY(EditAnywhere, meta = (UseComponentPicker))
	FComponentReference ItemAttachParent;

	UFUNCTION(BlueprintCallable)
	void EquipSlot(int Slot);

	UFUNCTION(BlueprintCallable)
	void UnequipItem();

	UFUNCTION(BlueprintCallable)
	void DropItemFromSlot(int Slot, int Count);

	UFUNCTION(BlueprintCallable)
	void RemoveFromInventory(int ItemSlot, const int Count);

	UFUNCTION(BlueprintCallable)
	int FindItemSlot(UItemData* Item);

private:
	UFUNCTION(BlueprintCallable)
	bool AddToInventory(UItemData* Item, const int Count);
		
};
