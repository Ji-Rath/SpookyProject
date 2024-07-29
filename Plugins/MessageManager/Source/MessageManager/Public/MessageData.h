// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MessageData.generated.h"

USTRUCT(BlueprintType)
struct FSimpleTextMessage : public FTableRowBase
{
	GENERATED_BODY()

	/** The message to display */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Message")
	FText Message;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Message")
	USoundBase* Sound;
};
