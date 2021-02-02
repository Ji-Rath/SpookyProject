#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemWidget.generated.h"

class UTextBlock;

/**
 *
 */
UCLASS()
class SPOOKYGAME_API UInventoryItemWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void UpdateDisplay(const FText& Name, const int& Amount);

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemAmount;
};