#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsGrabComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGrabUpdate, bool, bGrab, AInteractableBase*, Actor);

class UPhysicsHandleComponent;

/**
 * Allows a pawn to grab a physics body through the use of a physics handle component
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPOOKYGAME_API UPhysicsGrabComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPhysicsGrabComponent();

	void BeginPlay() override;

	/** Called to grab the in-range physics object */
	void Grab();

	UPROPERTY()
	FGrabUpdate OnGrabUpdate;

private:
	UPROPERTY()
	UPhysicsHandleComponent* HandleRef;

	UPROPERTY()
	APawn* Player;

	UPROPERTY(EditAnywhere, Category = "Grab")
	float InteractDistance = 100.f;
};


