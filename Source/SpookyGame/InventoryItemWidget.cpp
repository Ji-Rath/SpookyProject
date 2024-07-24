
#include "InventoryItemWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Inventory/PlayerEquipComponent.h"
#include "Inventory/InventoryComponent.h"
#include "Interaction/ItemData.h"

bool UInventoryItemWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }

	if (ItemButton)
		ItemButton->OnClicked.AddDynamic(this, &UInventoryItemWidget::ClickItem);
	return true;
}

void UInventoryItemWidget::UpdateDisplay(const FText& Name, const int& Amount)
{
	if (ensure(ItemName && ItemAmount))
	{
		ItemName->SetText(Name);
		ItemAmount->SetText(FText::AsNumber(Amount));
	}
}

void UInventoryItemWidget::ClickItem()
{
	OnClick.Broadcast(ItemSlot);
}
