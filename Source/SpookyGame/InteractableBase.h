// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableBase.generated.h"

UCLASS()
class SPOOKYGAME_API AInteractableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Determine whether anything can interact with the actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bCanInteract = true;

	//Determine whether the player can interact with the actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bPlayerInteract = false;

	//The name of the actor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Text")
	FText Name;

	//A short description of the actor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Text")
	FText Description;

	//Whether the actor can only be used once
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool bOneTimeInteraction = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bIsOn = false;

	UFUNCTION()
	void ToggleOnStatus();

	UFUNCTION()
	bool CanInteract() const;

	UFUNCTION()
	void SetInteractable(bool Interactable);
};
