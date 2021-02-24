// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NotificationManagerWidget.generated.h"

class UHorizontalBox;

/**
 * 
 */
UCLASS()
class SPOOKYGAME_API UNotificationManagerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	bool Initialize() override;

	UFUNCTION()
	void InventoryChange(bool bAdded, int SlotChanged);

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* NotificationHolder;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> IconWidget;
};
