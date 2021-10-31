// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TriggerComponent.generated.h"

class AInteractable;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INTERACTIONSYSTEM_API UTriggerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTriggerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	/** TArray of actors to trigger when TriggerActors() function is called */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger")
	TArray<AInteractable*> ActorsToTrigger;

	/** Function to be called to trigger actors in ActorsToTrigger array */
	UFUNCTION(BlueprintCallable)
	void TriggerActors(AActor* Instigator);

	/** Delay before triggering actors, 0 for no delay */
	UPROPERTY(EditAnywhere, Category = "Trigger|Advanced")
	float TriggerDelay = 0.f;

	/** Times to allow triggering */
	UPROPERTY(EditAnywhere, Category = "Trigger|Advanced")
	int TriggerAmount = 0;

private:
	/** Internal variable for tracking times triggered */
	int TriggerCount = 0;

	/** Handle trigger delay timer */
	FTimerHandle DelayHandle;

	/** Actual functionality for triggering actors */
	UFUNCTION()
	void ExecuteInteraction(AActor* Instigator);
};
