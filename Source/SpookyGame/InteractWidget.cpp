
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

void UInteractWidget::UpdateUI(TWeakObjectPtr<UPrimitiveComponent> Interactable)
{
	if (Interactable.IsValid() && Interactable->GetOwner()->Implements<UInteractable>())
	{
		PlayAnimation(MessageFade, 0.f, 1.f, EUMGSequencePlayMode::Forward, 1.f, true);
	}
	else
	{
		PlayAnimation(MessageFade, 0.f, 1.f, EUMGSequencePlayMode::Reverse, 1.f, true);
	}
}
