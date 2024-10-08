#include "ReadableWidget.h"

#include "Interaction/ItemData.h"
#include "Components/TextBlock.h"
#include "SpookyGame/PlayerControllerBase.h"
#include "SpookyGame/Interactables/Readable.h"

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
	if (BookData.PageData.Num() > 0)
	{
		CurrentPage = FMath::Clamp(CurrentPage + Num, 0, BookData.PageData.Num()-1);
		TextBody->SetText(BookData.PageData[CurrentPage]);
		UpdatePageArrows();
	}
}

void UReadableWidget::UpdateReadable(const FReadableData& Readable)
{
	BookData = Readable;
	
	if (!GetWidgetVisibility() && !BookData.PageData.IsEmpty())
	{
		IncrementPage(0);
		SetWidgetVisibility(true);
		//PlayerController->SetMouseState(false);
	}
}

bool UReadableWidget::DoesPageExist(int Page, bool bRelative)
{
	bool bPageExists = false;
	
	int Pages = BookData.PageData.Num();
	int PageToCheck = 0;
	if (bRelative)
	{
		PageToCheck = CurrentPage;
	}
	PageToCheck += Page;
	bPageExists = BookData.PageData.IsValidIndex(PageToCheck);
	return bPageExists;
}
