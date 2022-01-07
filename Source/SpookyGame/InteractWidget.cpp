
// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractWidget.h"
#include "Components/TextBlock.h"
#include "Interaction/PlayerInteractComponent.h"
#include "Interaction/Interactable.h"
#include "Interaction/PhysicsGrabComponent.h"

bool UInteractWidget::Initialize()
{
	// Dont continue if initialization failed
	bool Success = Super::Initialize();
	if (!Success) { return false; }
	
	// Attempt to bind delegates to functions to update UI
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

void UInteractWidget::UpdateUI(bool bShowCursor, AInteractable* Interactable)
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
		CurrentInteractable = Interactable;
		if (Interactable)
		{
			if (!CurrentInteractable->GetName().IsEmpty())
			{
				InteractText->SetText(CurrentInteractable->GetName());
				PlayAnimation(MessageFade, 0.0f, 1, EUMGSequencePlayMode::Forward);
				CurrentPlayModeMessage = EUMGSequencePlayMode::Forward;
			}
		}
		else if (CurrentPlayModeMessage != EUMGSequencePlayMode::Reverse)
		{
			PlayAnimation(MessageFade, 0.0f, 1, EUMGSequencePlayMode::Reverse);
			CurrentPlayModeMessage = EUMGSequencePlayMode::Reverse;
		}
	}
}
