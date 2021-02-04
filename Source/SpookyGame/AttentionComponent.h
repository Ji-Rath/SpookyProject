// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttentionComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPOOKYGAME_API UAttentionComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void ForceLookAt(AActor* LookAt, float Duration = 1.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float LookAtSpeed = 3.f;

private:
	//Actor that player is looking at
	AActor* LookingAt = nullptr;

	//Timer to handle looking at an object
	FTimerHandle LookAtTimer;

	void HandleLookingAt(float DeltaTime);

	void ClearLookingAt();
};