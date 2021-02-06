#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AdvCharacterMovementComponent.generated.h"

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CUSTOM_Sprint    UMETA(DisplayName = "Sprint")
};

UCLASS()
class SPOOKYGAME_API UAdvCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	/** The maximum ground speed when walking. Also determines maximum lateral speed when falling. */
	UPROPERTY(Category="Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0"))
	float MaxSprintSpeed;

	void PhysCustom(float deltaTime, int32 Iterations) override;

	bool IsSprinting();
};

