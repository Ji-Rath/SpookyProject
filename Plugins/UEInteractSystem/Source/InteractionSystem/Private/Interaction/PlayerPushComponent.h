#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerPushComponent.generated.h"

class UCharacterMovementComponent;
class UPhysicsHandleComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPushObject, bool, bPushingObject);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INTERACTIONSYSTEM_API UPlayerPushComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	
	UPlayerPushComponent();

	void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Max move speed while pushing an object */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PushMaxSpeed = 100.f;

	/* Minimum distance the pushed object has to be from the player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinObjectDistance = 75.f;

	/* Maximum distance the pushed object can be from the player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxObjectDistance = 125.f;

	/* Pushed Object offset relative to the player */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector TargetOffset;

	/* Determines whether an object is being pushed */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bPushingObject;

	/* Rotation facing the pushed object relative to the player */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FRotator TargetRotation;

	/* Called every time the player begins/ends pushing an object */
	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	FPushObject OnPushObject;

	/* Begin pushing an object */
	UFUNCTION(BlueprintCallable)
	void PushObject(UPrimitiveComponent* PushComponent);

	/* Stop pushing the object */
	UFUNCTION(BlueprintCallable)
	void StopPushing();

	/* Returns whether an object is currently being pushed */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsPushingObject();

	UCharacterMovementComponent* CharacterMoveComp;

	UPhysicsHandleComponent* PhysicsHandle;
};

