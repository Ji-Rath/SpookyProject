#include "Interaction/ToggleInteractable.h"

void AToggleInteractable::ToggleInteract(AActor* Interactor)
{
	SetState(Interactor, !bIsOn);
}

bool AToggleInteractable::Interact(AActor* Interactor)
{
	bIsOn = !bIsOn;
	bool bSuccess = Super::Interact(Interactor);

	if (!bSuccess)
	{
		bIsOn = !bIsOn;
	}

	return bSuccess;
}

void AToggleInteractable::SetState(AActor* Interactor, bool bOn)
{
	/** Ensure that current state is different and that interaction is possible */
	if (bOn != bIsOn)
	{
		Interact(Interactor);
	}
		
}

void AToggleInteractable::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	ConstructInteractable();
}

bool AToggleInteractable::GetState() const
{
	return bIsOn;
}

void AToggleInteractable::ConstructInteractable_Implementation()
{
	SetState(this, bIsOn);
}
