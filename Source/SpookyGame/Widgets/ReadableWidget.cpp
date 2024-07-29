#include "ReadableWidget.h"

#include "SpookyGame/BookData.h"
#include "Inventory/PlayerEquipComponent.h"
#include "Interaction/ItemData.h"
#include "Components/TextBlock.h"
#include "SpookyGame/PlayerControllerBase.h"

class UItemDataComponent;

bool UReadableWidget::GetWidgetVisibility()
{
	return bVisible;
}

bool UReadableWidget::Initialize()
{
	bool bSuccess = Super::Initialize();
	if (!bSuccess) { return false; }

	return true;
}

void UReadableWidget::ToggleVisibility()
{
	SetWidgetVisibility(!bVisible);
}

void UReadableWidget::SetWidgetVisibility(bool bNewVisibility)
{
	auto PlayMode = bNewVisibility ? EUMGSequencePlayMode::Reverse : EUMGSequencePlayMode::Forward;

	if (bVisible != bNewVisibility)
	{
		PlayAnimation(FadeAnimation, 0.0f, 1, PlayMode);
		bVisible = bNewVisibility;
	}

	// Reset page index
	if (bNewVisibility == true)
		CurrentPage = 0;
}

int UReadableWidget::GetPageIndex()
{
	return CurrentPage;
}

void UReadableWidget::IncrementPage(int Num)
{
	if (BookData->PageData.Num() > 0)
	{
		CurrentPage = FMath::Clamp(CurrentPage + Num, 0, BookData->PageData.Num()-1);
		TextBody->SetText(BookData->PageData[CurrentPage]);
		UpdatePageArrows();
	}
}

void UReadableWidget::OnUseItem(const FInventoryContents& ItemName)
{
	if (auto* BookInfo = ItemName.GetItemInformation<UBookData>())
		BookData = BookInfo;
	
	if (!GetWidgetVisibility() && BookData && !BookData->PageData.IsEmpty())
	{
		IncrementPage(0);
		SetWidgetVisibility(true);
		PlayerController->SetMouseState(false);
		TextTitle->SetText(BookData->DisplayName);
	}
}

void UReadableWidget::OnItemInteract(UItemInformation* ItemInfo)
{
	if (!ItemInfo) { return; }
	
	/** Ensure interacted object is a viewable item */
}

bool UReadableWidget::DoesPageExist(int Page, bool bRelative)
{
	bool bPageExists = false;
	
	int Pages = BookData->PageData.Num();
	int PageToCheck = 0;
	if (bRelative)
	{
		PageToCheck = CurrentPage;
	}
	PageToCheck += Page;
	bPageExists = BookData->PageData.IsValidIndex(PageToCheck);
	return bPageExists;
}
