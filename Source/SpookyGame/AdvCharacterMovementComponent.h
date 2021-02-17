#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AdvCharacterMovementComponent.generated.h"

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CUSTOM_Sprint    UMETA(DisplayName = "Sprint")
};

/**
 * Custom movement component that includes additional functionality like sprinting
 */
UCLASS()
class SPOOKYGAME_API UAdvCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	void PhysCustom(float deltaTime, int32 Iterations) override;

	/** Sprinting functionality */
	void PhysSprinting(float deltaTime, int32 Iterations);

	/** Returns whether the character is in the sprinting state */
	bool IsSprinting();

private:
	/** The maximum ground speed when walking. Also determines maximum lateral speed when falling. */
	UPROPERTY(Category="Character Movement: Walking", EditAnywhere, meta=(ClampMin="0", UIMin="0"))
	float MaxSprintSpeed;
};

