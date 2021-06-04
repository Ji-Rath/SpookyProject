// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"
#include "MainUIBase.h"

void APlayerControllerBase::ToggleInventory()
{
	MainUI->ToggleInventory();
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
