#include "ReadableWidget.h"
#include "PlayerEquipComponent.h"
#include "BookData.h"
#include "ItemData.h"
#include "Components/TextBlock.h"
#include "PlayerControllerBase.h"

bool UReadableWidget::GetWidgetVisibility()
{
	return bVisible;
}

bool UReadableWidget::Initialize()
{
	bool bSuccess = Super::Initialize();
	if (!bSuccess) { return false; }

	PlayerController = GetOwningPlayer<APlayerControllerBase>();
	if (PlayerController)
	{
		APawn* Player = PlayerController->GetPawn();
		if (Player)
		{
			auto* EquipComponent = Player->FindComponentByClass<UPlayerEquipComponent>();
			if (EquipComponent)
			{
				EquipComponent->OnItemUse.AddDynamic(this, &UReadableWidget::OnUseItem);
			}
		}
	}

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
}

int UReadableWidget::GetPageIndex()
{
	return CurrentPage;
}

void UReadableWidget::IncrementPage(int Num)
{
	if (BookData)
	{
		CurrentPage = FMath::Clamp(Num + CurrentPage, 0, BookData->Text.Num());
		TextBody->SetText(BookData->Text[CurrentPage]);
	}
}

void UReadableWidget::OnUseItem(UItemData* ItemData)
{
	BookData = Cast<UBookData>(ItemData);
	if (BookData && !GetWidgetVisibility())
	{
		IncrementPage(0);
		SetWidgetVisibility(true);
		PlayerController->SetMouseState(false);
	}
}
