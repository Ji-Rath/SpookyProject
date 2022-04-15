// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBase.generated.h"

class UMainUIBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseLock, bool, bLocked);

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

	/** Set the state of the mouse input (For UI related purposes) */
	UFUNCTION(BlueprintCallable)
	void SetMouseState(bool bLocked);

	/** Toggle the state of the mouse input */
	UFUNCTION(BlueprintCallable)
	void ToggleMouseState();

	FMouseLock OnMouseLock;
};
