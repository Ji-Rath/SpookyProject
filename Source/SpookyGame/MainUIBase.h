// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUIBase.generated.h"

/**
 * 
 */
UCLASS()
class SPOOKYGAME_API UMainUIBase : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void PlayInteractAnim(EUMGSequencePlayMode::Type PlayMode);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TEnumAsByte<EUMGSequencePlayMode::Type> CurrentPlayMode;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ToggleInventory();
};
