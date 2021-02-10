
// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractWidget.h"
#include "Components/TextBlock.h"
#include "PlayerInteractComponent.h"

bool UInteractWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }
	
	if (GetOwningPlayerPawn())
	{
		UPlayerInteractComponent* InteractComponent = GetOwningPlayerPawn()->FindComponentByClass<UPlayerInteractComponent>();
		if (InteractComponent)
			InteractComponent->OnUpdateInteract.AddDynamic(this, &UInteractWidget::UpdateText);
	}

	return true;
}

void UInteractWidget::UpdateText(FString NewText)
{
	if (NewText != "")
	{
		InteractText->SetText(FText::FromString(NewText));
		if (CurrentPlayMode != EUMGSequencePlayMode::Forward)
		{
			CurrentPlayMode = EUMGSequencePlayMode::Forward;
			PlayAnimation(InteractionFade);
		}
	}
	else if (CurrentPlayMode != EUMGSequencePlayMode::Reverse)
	{
		CurrentPlayMode = EUMGSequencePlayMode::Reverse;
		PlayAnimation(InteractionFade, 0.0f, 1, CurrentPlayMode);
	}
}
