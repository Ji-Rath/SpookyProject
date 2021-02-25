#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemWidget.generated.h"

class UTextBlock;
class UButton;
class UPlayerEquipComponent;
class UInventoryComponent;

/**
 *
 */
UCLASS()
class SPOOKYGAME_API UInventoryItemWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;

	UFUNCTION()
	void UpdateDisplay(const FText& Name, const int& Amount);

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemAmount;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton;

	UPROPERTY();
	int ItemSlot = 0;

	UFUNCTION()
	void ToggleItem();
private:
	UPROPERTY()
	UPlayerEquipComponent* EquipCompRef;

	UPROPERTY()
	UInventoryComponent* InventoryRef;
};