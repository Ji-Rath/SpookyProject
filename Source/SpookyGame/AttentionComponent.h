// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttentionComponent.generated.h"

/**
 * Allows a possessed pawn to 'forcefully' look in a specific direction or actor
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPOOKYGAME_API UAttentionComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UAttentionComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * Look at the specified actor for a set duration
	 * @param LookAtActor - Actor to look at
	 * @param Duration - Length in seconds to look at actor
	*/
	UFUNCTION(BlueprintCallable)
	void LookAt(AActor* LookAtActor, float Duration = 1.f);

	/** Clear any duration to look at an actor */
	UFUNCTION()
	void ClearLookingAt();

private:
	//Actor that player is looking at
	AActor* LookingAt = nullptr;

	//Timer to handle looking at an object
	FTimerHandle LookAtTimer;

	void HandleLookingAt(float DeltaTime);

	UPROPERTY(EditDefaultsOnly)
	float LookAtStrength = 3.f;
};