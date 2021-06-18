// Fill out your copyright notice in the Description page of Project Settings.


#include "NotificationManagerWidget.h"
#include "Components/HorizontalBox.h"
#include "Inventory/InventoryComponent.h"

bool UNotificationManagerWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }

	if (GetOwningPlayerPawn())
	{
		UInventoryComponent* InventoryComponent = GetOwningPlayerPawn()->FindComponentByClass<UInventoryComponent>();
		InventoryComponent->OnInventoryChange.AddDynamic(this, &UNotificationManagerWidget::InventoryChange);
	}
	return true;
}

void UNotificationManagerWidget::InventoryChange(bool bAdded)
{
	if (ensure(IconWidget) && bAdded)
	{
		UWidget* Icon = CreateWidget(NotificationHolder, IconWidget);
		NotificationHolder->AddChild(Icon);
	}
}
