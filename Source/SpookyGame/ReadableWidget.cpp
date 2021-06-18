#include "ReadableWidget.h"
#include "Inventory/PlayerEquipComponent.h"
#include "BookData.h"
#include "Interaction/ItemData.h"
#include "Components/TextBlock.h"
#include "PlayerControllerBase.h"
#include "Interaction/PlayerInteractComponent.h"
#include "Viewable.h"

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

			UPlayerInteractComponent* InteractComponent = Player->FindComponentByClass<UPlayerInteractComponent>();
			if (InteractComponent)
			{
				InteractComponent->OnInteract.AddDynamic(this, &UReadableWidget::OnItemInteract);
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
	if (BookData && BookData->Text.Num() > 0)
	{
		CurrentPage = FMath::Clamp(CurrentPage + Num, 0, BookData->Text.Num()-1);
		TextBody->SetText(BookData->Text[CurrentPage]);
	}
}

void UReadableWidget::OnUseItem(UItemData* ItemData)
{
	BookData = Cast<UBookData>(ItemData);
	if (BookData && !GetWidgetVisibility() && ensure(BookData->Text.Num() > 0))
	{
		IncrementPage(0);
		SetWidgetVisibility(true);
		PlayerController->SetMouseState(false);
	}
}

void UReadableWidget::OnItemInteract(AInteractable* Interactable)
{
	/** Ensure interacted object is a viewable item */
	if (Cast<AViewable>(Interactable))
	{
		OnUseItem(Interactable->GetItemData());
	}
}
