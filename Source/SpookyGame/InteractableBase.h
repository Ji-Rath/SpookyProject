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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanInteract = true;

	//Determine whether the player can interact with the actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPlayerInteract = false;

	//The name of the actor
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name;

	//A short description of the actor
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Description;

	//Whether the actor can only be used once
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bOneTimeInteraction = false;

};
