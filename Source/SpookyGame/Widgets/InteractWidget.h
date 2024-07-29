// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractWidget.generated.h"

class AInteractable;
class UTextBlock;

/**
 * 
 */
UCLASS()
class SPOOKYGAME_API UInteractWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	bool Initialize() override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetInteractable(UPrimitiveComponent* Component);

	UFUNCTION()
	void UpdateUI(const TWeakObjectPtr<UPrimitiveComponent>& Interactable);

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* InteractionFade;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* MessageFade;

	bool bInteractionVisible = false;
	bool bMessageVisible = false;
};
