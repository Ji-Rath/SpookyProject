// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interaction/InteractableComponent.h"
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

	UFUNCTION(BlueprintCallable)
	void UpdateUI(bool bShowCursor, UInteractableComponent* Interactable);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TEnumAsByte<EUMGSequencePlayMode::Type> CurrentPlayMode = EUMGSequencePlayMode::PingPong;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TEnumAsByte<EUMGSequencePlayMode::Type> CurrentPlayModeMessage = EUMGSequencePlayMode::PingPong;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* InteractionFade;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* MessageFade;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInteractableComponent* CurrentInteractable = nullptr;
};
