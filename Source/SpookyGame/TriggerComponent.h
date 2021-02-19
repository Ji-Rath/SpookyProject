// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TriggerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPOOKYGAME_API UTriggerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTriggerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	//TArray of actors to trigger when TriggerActors() function is called
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<AActor*> ActorsToTrigger;

	//Function to be called to trigger actors in ActorsToTrigger array
	UFUNCTION(BlueprintCallable)
	void TriggerActors(AActor* Instigator);
};
