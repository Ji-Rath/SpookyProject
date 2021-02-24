// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBase.generated.h"

class UMainUIBase;

/**
 * 
 */
UCLASS()
class SPOOKYGAME_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMainUIBase* MainUI;
	
	UFUNCTION(BlueprintCallable)
	void ToggleInventory();
};
