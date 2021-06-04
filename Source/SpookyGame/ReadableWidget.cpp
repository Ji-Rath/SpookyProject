#include "ReadableWidget.h"
#include "PlayerEquipComponent.h"
#include "BookData.h"
#include "ItemData.h"
#include "Components/TextBlock.h"

bool UReadableWidget::Initialize()
{
	bool bSuccess = Super::Initialize();
	if (!bSuccess) { return false; }

	if (APawn* Player = GetOwningPlayerPawn())
	{
		auto* EquipComponent = Player->FindComponentByClass<UPlayerEquipComponent>();
		if (EquipComponent)
		{
			EquipComponent->OnItemUse.AddDynamic(this, &UReadableWidget::OnUseItem);
		}
	}

	return true;
}

void UReadableWidget::ToggleAnimation()
{
	auto PlayMode = CurrentPlayMode == EUMGSequencePlayMode::Forward ? EUMGSequencePlayMode::Reverse : EUMGSequencePlayMode::Forward;
	SetAnimationState(PlayMode);
}

void UReadableWidget::SetAnimationState(EUMGSequencePlayMode::Type PlayMode)
{
	if (CurrentPlayMode != PlayMode)
	{
		PlayAnimation(FadeAnimation, 0.0f, 1, PlayMode);
		CurrentPlayMode = PlayMode;
	}
}

void UReadableWidget::OnUseItem(UItemData* ItemData)
{
	UBookData* BookData = Cast<UBookData>(ItemData);
	if (BookData)
	{
		TextBody->SetText(BookData->Text);
		ToggleAnimation();
	}
}
