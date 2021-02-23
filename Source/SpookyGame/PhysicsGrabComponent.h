#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsGrabComponent.generated.h"

class ITriggerInterface;
class UPhysicsHandleComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGrabUpdate, bool, bGrab, TScriptInterface<ITriggerInterface>, Actor);

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

	UPROPERTY(EditDefaultsOnly)
	float GrabThreshhold = 25.f;

	UPROPERTY(EditDefaultsOnly)
	float PushForce = 100000.f;

	UPROPERTY(EditDefaultsOnly)
	float InitialForce = 500.f;

	UPROPERTY(EditDefaultsOnly)
	float PushDuration = 0.5f;

private:
	UPROPERTY()
	UPhysicsHandleComponent* HandleRef = nullptr;

	UPROPERTY()
	APawn* Player = nullptr;

	UPROPERTY(EditAnywhere, Category = "Grab")
	float InteractDistance = 100.f;

	FTimerHandle PushTimer;

	UStaticMeshComponent* PushComponent = nullptr;

	void StopPushing();
};


