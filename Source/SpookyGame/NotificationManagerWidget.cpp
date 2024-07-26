// Fill out your copyright notice in the Description page of Project Settings.


#include "NotificationManagerWidget.h"
#include "Components/HorizontalBox.h"
#include "Inventory/InventoryComponent.h"

bool UNotificationManagerWidget::Initialize()
{
	bool Success = Super::Initialize();

	if (GetOwningPlayerPawn())
	{
		if (UInventoryComponent* InventoryComponent = GetOwningPlayerPawn()->FindComponentByClass<UInventoryComponent>())
		{
			InventoryComponent->OnInventoryChange.AddDynamic(this, &UNotificationManagerWidget::InventoryChange);
		}
	}
	return Success;
}

void UNotificationManagerWidget::CreateNotification_Implementation(TSubclassOf<UUserWidget> NotificationWidget)
{
	if (ensure(IsValid(NotificationWidget)))
	{
		UWidget* Icon = CreateWidget(NotificationHolder, IconWidget);
		NotificationHolder->AddChild(Icon);
	}
	
	
}

void UNotificationManagerWidget::InventoryChange(const TArray<FInventoryContents>& Inventory)
{
	CreateNotification(IconWidget);
}
