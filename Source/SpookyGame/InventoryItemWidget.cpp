
#include "InventoryItemWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "InventoryComponent.h"

bool UInventoryItemWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }

	if (GetOwningPlayerPawn())
		InventoryRef = GetOwningPlayerPawn()->FindComponentByClass<UInventoryComponent>();

	if (ItemButton)
		ItemButton->OnClicked.AddDynamic(this, &UInventoryItemWidget::ToggleItem);
	return true;
}

void UInventoryItemWidget::UpdateDisplay(const FText& Name, const int& Amount)
{
	ItemName->SetText(Name);
	ItemAmount->SetText(FText::AsNumber(Amount));
}

void UInventoryItemWidget::ToggleItem()
{
	if (InventoryRef)
	{
		/** Equip current item in slot if not already done */
		if (InventoryRef->GetEquippedSlot() != ItemSlot)
		{
			InventoryRef->EquipSlot(ItemSlot);
		}
		else
		{
			InventoryRef->UnequipItem();
		}
	}
}
