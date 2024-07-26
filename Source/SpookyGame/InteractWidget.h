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

	UPROPERTY(meta = (BindWidget))
	UTextBlock* InteractText;

	UFUNCTION()
	void UpdateUI(TWeakObjectPtr<UPrimitiveComponent> Interactable);

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* InteractionFade;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* MessageFade;
};
