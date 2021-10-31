#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsGrabComponent.generated.h"

class IInteraction;
class UPhysicsHandleComponent;
class AInteractable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGrabUpdate, bool, bGrab, AInteractable*, Actor);

/**
 * Allows a pawn to grab a physics body through the use of a physics handle component
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INTERACTIONSYSTEM_API UPhysicsGrabComponent : public UActorComponent
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
	UPhysicsHandleComponent* HandleRef = nullptr;

	UPROPERTY()
	APawn* Player = nullptr;

	UPROPERTY(EditAnywhere, Category = "Physics")
	float InteractDistance = 100.f;

	UPROPERTY(EditAnywhere, Category = "Physics|Grab")
	float GrabRange = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Physics|Grab")
	float GrabWeightThreshhold = 25.f;

	UPROPERTY(EditDefaultsOnly, Category = "Physics|Push")
	float PushImpulse = 100000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Physics|Grab")
	float MaxDistance = 100.f;

	float CurrentGrabDistance = 0.f;

	FTimerHandle PushTimer;

	void StopPushing();

	UPROPERTY(EditDefaultsOnly, Category = "Physics|Push")
	float PushDelay = 1.f;
};


