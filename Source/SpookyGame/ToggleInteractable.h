// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "ToggleInteractable.generated.h"

class UToggleInteractableComponent;
UCLASS()
class SPOOKYGAME_API AToggleInteractable : public AInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AToggleInteractable(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interactable")
	UToggleInteractableComponent* GetToggleInteractableComponent() const;
};
