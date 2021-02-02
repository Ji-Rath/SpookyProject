
#include "InventoryItemWidget.h"
#include "Components/TextBlock.h"

void UInventoryItemWidget::UpdateDisplay(const FText& Name, const int& Amount)
{
	ItemName->SetText(Name);
	ItemAmount->SetText(FText::AsNumber(Amount));
}
