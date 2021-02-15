// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractWidget.generated.h"

class UTextBlock;
class AInteractableBase;

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
	void UpdateUI(bool bShowCursor, AInteractableBase* Interactable);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void PlayInteractAnim(EUMGSequencePlayMode::Type PlayMode);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TEnumAsByte<EUMGSequencePlayMode::Type> CurrentPlayMode;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* InteractionFade;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* MessageFade;
private:
	AInteractableBase* CurrentInteractable = nullptr;
};
