#include "AdvCharacterMovementComponent.h"

void UAdvCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	switch (CustomMovementMode)
	{
	case CUSTOM_Sprint:
		PhysSprinting(deltaTime, Iterations);
		break;
	}
}

void UAdvCharacterMovementComponent::PhysSprinting(float deltaTime, int32 Iterations)
{
	MaxCustomMovementSpeed = MaxSprintSpeed;
	PhysWalking(deltaTime, Iterations);
}

bool UAdvCharacterMovementComponent::IsSprinting()
{
	return MovementMode == MOVE_Custom && CustomMovementMode == CUSTOM_Sprint;
}

