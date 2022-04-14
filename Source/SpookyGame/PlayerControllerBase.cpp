// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"
#include "Components/WidgetSwitcher.h"
#include "MainUIBase.h"
#include "Kismet/GameplayStatics.h"

void APlayerControllerBase::ToggleInventory()
{
	if (MainUI)
	{
		MainUI->ToggleInventory();
	}
}

void APlayerControllerBase::PauseGame()
{
	if (MainUI && MainUI->WidgetSelector)
	{
		MainUI->WidgetSelector->SetActiveWidgetIndex(1);
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		SetMouseState(false);
	}
}

void APlayerControllerBase::SetMouseState(bool bLocked)
{
	SetIgnoreLookInput(!bLocked);
	bShowMouseCursor = IsLookInputIgnored();

	if (bLocked && !IsLookInputIgnored())
	{
		SetInputMode(FInputModeGameOnly());
	}
	else
	{
		SetInputMode(FInputModeGameAndUI());
	}

	OnMouseLock.Broadcast(bLocked);
}

void APlayerControllerBase::ToggleMouseState()
{
	// Toggle mouse state based on cursor visibility
	SetMouseState(bShowMouseCursor);
}
