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
	virtual bool Initialize() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Notification")
	void CreateNotification(TSubclassOf<UUserWidget> NotificationWidget);

	UFUNCTION()
	void InventoryChange(bool bAdded);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* NotificationHolder;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> IconWidget;
};
