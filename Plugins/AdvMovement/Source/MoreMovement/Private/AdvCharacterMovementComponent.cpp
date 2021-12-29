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
	PhysWalking(deltaTime, Iterations);
}

bool UAdvCharacterMovementComponent::IsSprinting() const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == CUSTOM_Sprint && !IsCrouching();
}

bool UAdvCharacterMovementComponent::IsMovingOnGround() const
{
	bool bMoving = Super::IsMovingOnGround();
	return (bMoving || (MovementMode == MOVE_Custom && CustomMovementMode == CUSTOM_Sprint)) && UpdatedComponent;
}

void UAdvCharacterMovementComponent::Sprint()
{
	SetMovementMode(MOVE_Custom, CUSTOM_Sprint);
}

void UAdvCharacterMovementComponent::StopSprint()
{
	SetDefaultMovementMode();
}

float UAdvCharacterMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();
	const float Speed = IsCrouching() ? MaxWalkSpeedCrouched : MaxWalkSpeed;

	switch(MovementMode)
	{
	case MOVE_Custom:
		MaxSpeed = IsSprinting() ? MaxSprintSpeed : Speed;
		break;
	default:
		break;
	}

	return MaxSpeed * SpeedMultiplier;
}

