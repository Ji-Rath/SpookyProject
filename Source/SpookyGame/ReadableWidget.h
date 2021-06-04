#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReadableWidget.generated.h"

class UItemData;
class UTextBlock;
class APlayerControllerBase;

UCLASS()
class SPOOKYGAME_API UReadableWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	bool GetWidgetVisibility();

	/** Toggle the state of the widget by reversing the widget animation */
	UFUNCTION(BlueprintCallable)
	void ToggleVisibility();

	/** Set the widget to the specified state by playing widget animation forwards/backwards */
	UFUNCTION(BlueprintCallable)
	void SetWidgetVisibility(bool bNewVisibility);

protected:
	bool Initialize() override;

	/** The current playmode of the widget */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bVisible = false;

	/** Widget Animation to play between states */
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeAnimation;

	/** Textblock to display item contents */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBody;

	/** Called whenever the player uses an item (on self) */
	UFUNCTION()
	void OnUseItem(UItemData* ItemData);

	UPROPERTY(BlueprintReadOnly)
	APlayerControllerBase* PlayerController;
};
