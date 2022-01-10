#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReadableWidget.generated.h"

class UItemData;
class UTextBlock;
class APlayerControllerBase;
class UBookData;
class AInteractable;

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

	UFUNCTION(BlueprintCallable)
	int GetPageIndex();

	/**
	 * Increment the page index and display the new text
	 * @param Num How many pages to increment
	 * @return void
	*/
	UFUNCTION(BlueprintCallable)
	void IncrementPage(int Num);

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

	/** Textblock to display item contents */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextTitle;

	/** Called whenever the player uses an item (on self) */
	UFUNCTION()
	void OnUseItem(UItemData* ItemData);

	/** Called whenever the player interacts with an item in the world */
	UFUNCTION()
	void OnItemInteract(AInteractable* Interactable);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	APlayerControllerBase* PlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int CurrentPage = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBookData* BookData = nullptr;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool DoesPageExist(int Page, bool bRelative);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdatePageArrows();
};
