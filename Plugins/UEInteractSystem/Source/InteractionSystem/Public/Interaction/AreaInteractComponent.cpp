
#include "AreaInteractComponent.h"
#include "Interaction/Interactable.h"
#include "Components/ShapeComponent.h"



void UAreaInteractComponent::HoverInteraction(float DeltaTime)
{
	/** Ensure that the owner is a pawn */
	APawn* Player = GetOwner<APawn>();
	if (!Player) return;
	APlayerController* PlayerController = Player->GetController<APlayerController>();
	if (!PlayerController) return;

	UShapeComponent* ShapeComp = Cast<UShapeComponent>(OverlapVolume.GetComponent(GetOwner()));
	if (ensure(ShapeComp))
	{
		ShapeComp->GetOverlappingActors(OverlappingActors, AInteractable::StaticClass());
		
		AInteractable* OverlappedActor = nullptr;
		if (OverlappingActors.Num() > 0)
		{
			OverlappedActor = Cast<AInteractable>(OverlappingActors[0]);

			if (OverlappedActor)
			{
				/** Return early if there is no change */
				if (OverlappedActor == InteractHover) { return; }

				/** Set interact message when hovering over an interactable */
				if (OverlappedActor->CanInteract(GetOwner()))
				{
					InteractHover = OverlappedActor;
					OnUpdateInteract.Broadcast(true, InteractHover);
					return;
				}
			}
		}

		/** Send interaction update when there is no longer an interactable in view */
		if (InteractHover && OverlappedActor == nullptr)
		{
			InteractHover = nullptr;
			OnUpdateInteract.Broadcast(false, nullptr);
		}
	}
}
