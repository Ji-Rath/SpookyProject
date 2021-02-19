
// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractWidget.h"
#include "Components/TextBlock.h"
#include "PlayerInteractComponent.h"
#include "TriggerInterface.h"
#include "PhysicsGrabComponent.h"

bool UInteractWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) { return false; }
	
	if (APawn* Player = GetOwningPlayerPawn())
	{
		UPlayerInteractComponent* InteractComponent = Player->FindComponentByClass<UPlayerInteractComponent>();
		if (InteractComponent)
			InteractComponent->OnUpdateInteract.AddDynamic(this, &UInteractWidget::UpdateUI);
		UPhysicsGrabComponent* GrabComponent = Player->FindComponentByClass<UPhysicsGrabComponent>();
		if (GrabComponent)
			GrabComponent->OnGrabUpdate.AddDynamic(this, &UInteractWidget::UpdateUI);
	}

	return true;
}

void UInteractWidget::UpdateUI(bool bShowCursor, TScriptInterface<ITriggerInterface> Interactable)
{
	if (bShowCursor)
	{
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

	if (CurrentInteractable != Interactable)
	{
		if (Interactable)
		{
			InteractText->SetText(ITriggerInterface::Execute_GetName(Interactable.GetObject()));
			PlayAnimation(MessageFade, 0.0f, 1, EUMGSequencePlayMode::Forward);
		}
		else
		{
			PlayAnimation(MessageFade, 0.0f, 1, EUMGSequencePlayMode::Reverse);
		}
		CurrentInteractable = Interactable;
	}
}
