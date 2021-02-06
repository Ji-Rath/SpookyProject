#include "AdvCharacterMovementComponent.h"

void UAdvCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	switch (CustomMovementMode)
	{
	case CUSTOM_Sprint:
		MaxCustomMovementSpeed = MaxSprintSpeed;
		PhysWalking(deltaTime, Iterations);
		break;
	default:
		break;
	}
}

bool UAdvCharacterMovementComponent::IsSprinting()
{
	return MovementMode == MOVE_Custom && CustomMovementMode == CUSTOM_Sprint;
}

