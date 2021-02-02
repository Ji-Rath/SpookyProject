// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UInventoryComponent;
class UScrollBox;

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
	void UpdateInventory();

	UPROPERTY(EditAnywhere)
	UInventoryComponent* InventoryRef;
};
