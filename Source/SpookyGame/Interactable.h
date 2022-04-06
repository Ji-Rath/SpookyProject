// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ISpudObject.h"
#include "GameFramework/Actor.h"
#include "Interaction/InteractableComponent.h"
#include "Interactable.generated.h"

UCLASS(Blueprintable, BlueprintType)
class SPOOKYGAME_API AInteractable : public AActor, public ISpudObject, public ISpudObjectCallback
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractable(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SpudPreStore_Implementation(const USpudState* State) override;

	virtual void SpudPostRestore_Implementation(const USpudState* State) override;

	UPROPERTY(VisibleAnywhere, SaveGame)
	UInteractableComponent* InteractableComponent;

	static FName InteractableComponentName;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interactable")
	UInteractableComponent* GetInteractableComponent() const;
private:
	UPROPERTY(SaveGame)
	TArray< TSubclassOf<AActor> > ActorFilters;
	
	UPROPERTY(SaveGame)
	TArray< bool > ActorBoolFilters;
};
