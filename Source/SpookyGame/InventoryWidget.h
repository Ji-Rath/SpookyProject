// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UInventoryComponent;
class UScrollBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FItemClick, int, ItemIndex, UInventoryComponent*, Inventory);

/**
 * 
 */
UCLASS()
class SPOOKYGAME_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* InventoryDisplay;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> ItemWidget;

	UFUNCTION(BlueprintCallable)
	void UpdateInventory(const TArray<FInventoryContents>& Inventory);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn=true))
	UInventoryComponent* InventoryRef;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FItemClick OnItemClick;

	UFUNCTION()
	void ClickedOnItem(int ItemIndex);

	UFUNCTION()
	void ToggleItem(int ItemSlot);
};
