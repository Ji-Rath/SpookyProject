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

void UReadableWidget::OnUseItem(UItemData* ItemData)
{
	UBookData* BookData = Cast<UBookData>(ItemData);
	if (BookData && !GetWidgetVisibility())
	{
		TextBody->SetText(BookData->Text);
		SetWidgetVisibility(true);
		PlayerController->SetMouseState(false);
	}
}
