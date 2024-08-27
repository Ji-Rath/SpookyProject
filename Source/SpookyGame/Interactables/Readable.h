// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/Usable.h"
#include "Readable.generated.h"

// Data related to reading an item
USTRUCT(BlueprintType)
struct FReadableData
{
	GENERATED_BODY()
	
	// Page data for this readable item
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Info", meta=(MultiLine="true"))
	TArray<FText> PageData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Info")
	FSlateFontInfo Font;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Info")
	TSoftObjectPtr<UTexture2D> BackgroundTexture;
};

/**
 * A type of item that is readable by the user
 */
UCLASS()
class SPOOKYGAME_API UReadable : public UUsable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Info")
	FReadableData ReadableData;
};
