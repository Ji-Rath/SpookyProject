// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInteractComponent.generated.h"

class AInteractableBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPOOKYGAME_API UPlayerInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerInteractComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Distance that the player can interact with objects
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float InteractDistance = 500.f;

	//Store interact actor that the player is currently looking at
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AInteractableBase* InteractHover = nullptr;

	void Interact();

private:

	void HoverInteraction(float DeltaTime);
		
};