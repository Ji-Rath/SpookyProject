
#include "InventoryItemWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "PlayerEquipComponent.h"

bool UInventoryItemWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }

	if (GetOwningPlayerPawn())
		EquipCompRef = GetOwningPlayerPawn()->FindComponentByClass<UPlayerEquipComponent>();

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
	if (EquipCompRef)
	{
		/** Equip current item in slot if not already done */
		if (!EquipCompRef->HasItemEquipped())
		{
			EquipCompRef->EquipSlot(ItemSlot);
		}
		else 
		{
			if (EquipCompRef->GetEquippedSlot() == ItemSlot)
			{
				EquipCompRef->UnequipItem();
			}
			else
			{
				EquipCompRef->EquipSlot(ItemSlot);
			}
		}
	}
}
