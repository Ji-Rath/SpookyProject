
// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractWidget.h"
#include "Components/TextBlock.h"
#include "Interaction/Interactable.h"
#include "Interaction/InteractorComponent.h"
#include "Interaction/PhysicsGrabComponent.h"

bool UInteractWidget::Initialize()
{
	// Dont continue if initialization failed
	bool Success = Super::Initialize();
	if (!Success) { return false; }
	
	// Attempt to bind delegates to functions to update UI
	if (APawn* Player = GetOwningPlayerPawn())
	{
		UInteractorComponent* InteractComponent = Player->FindComponentByClass<UInteractorComponent>();
		if (InteractComponent)
			InteractComponent->OnUpdateHover.AddDynamic(this, &UInteractWidget::UpdateUI);
	}

	return true;
}

void UInteractWidget::UpdateUI(const TWeakObjectPtr<UPrimitiveComponent>& Interactable)
{
	bool bIsAnInteractable = Interactable.IsValid() && Interactable->GetOwner()->Implements<UInteractable>();
	if (bIsAnInteractable)
	{
		FText Name = IInteractable::Execute_GetName(Interactable->GetOwner(), Interactable.Get());
		SetInteractable(Interactable.Get());
		
		if (!bInteractionVisible)
		{
			PlayAnimation(InteractionFade, 0.f, 1.f, EUMGSequencePlayMode::Forward, 1.f, false);
			bInteractionVisible = true;
		}
		
		if (!bMessageVisible && !Name.IsEmpty())
		{
			PlayAnimation(MessageFade, 0.f, 1.f, EUMGSequencePlayMode::Forward, 1.f, false);
			bMessageVisible = true;
		}

		if (bMessageVisible && Name.IsEmpty())
		{
			PlayAnimation(MessageFade, 0.f, 1.f, EUMGSequencePlayMode::Reverse, 1.f, false);
			bMessageVisible = false;
		}
	}
	else
	{
		if (bInteractionVisible)
		{
			PlayAnimation(InteractionFade, 0.f, 1.f, EUMGSequencePlayMode::Reverse, 1.f, false);
			bInteractionVisible = false;
		}
		if (bMessageVisible)
		{
			PlayAnimation(MessageFade, 0.f, 1.f, EUMGSequencePlayMode::Reverse, 1.f, false);
			bMessageVisible = false;
		}
	}
}
